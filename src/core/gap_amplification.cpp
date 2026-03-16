#include <cmath>
#include <future>
#include <thread>
#include <unordered_map>
#include <vector>

#include "core/core.hpp"
#include "constants.hpp"
#include "pcpp/TesterFactory.hpp"
#include "util/disjoint_set_union.hpp"
#include "util/thread_pool.hpp"

void merge_variables(
    size_t original_size,
    pcp::BinaryCSP &pcp, 
    const std::vector<std::vector<std::pair<pcp::Variable, size_t>>> &occuring_location,
    const std::vector<pcp::BinaryCSP> &reduced_pcps
) {
    
    std::vector<size_t> offsets(1, 0);
    offsets.reserve(original_size + 1);
    for (const auto &reduced_pcp : reduced_pcps) {
        offsets.push_back(offsets.back() + reduced_pcp.get_size());
    }

    util::disjoint_set_union dsu(offsets.back());

    for (pcp::Variable v = 0; v < static_cast<pcp::Variable>(original_size); ++v) {
        if (occuring_location[v].size() <= 1) continue;
        const auto &locations = occuring_location[v];
        for (size_t i = 1; i < locations.size(); ++i) {
            pcp::Variable u1 = locations[0].first;
            size_t pos1 = locations[0].second;
            pcp::Variable u2 = locations[i].first;
            size_t pos2 = locations[i].second;

            // merge the variables in DSU
            dsu.merge(
                offsets[u1] + pos1 * 3,
                offsets[u2] + pos2 * 3
            );
            dsu.merge(
                offsets[u1] + pos1 * 3 + 1,
                offsets[u2] + pos2 * 3 + 1
            );
            dsu.merge(
                offsets[u1] + pos1 * 3 + 2,
                offsets[u2] + pos2 * 3 + 2
            );
        }
    }
    
    std::unordered_map<pcp::Variable, pcp::Variable> representative_map;
    size_t new_size = 0;
    for (pcp::Variable i = 0; i < offsets.back(); ++i) {
        if (dsu.find(i) == i) {
            representative_map[i] = new_size++;
        }
    }

    pcp::BinaryCSP new_BinaryCSP(new_size);

    for (pcp::Variable i = 0; i < offsets.back(); ++i) {
        if (dsu.find(i) == i) {
            new_BinaryCSP.set_variable(
                representative_map[i],
                pcp.get_variable(i)
            );
        }
    }

    for (const auto &[u, v, c] : pcp.get_constraints_list()) {
        pcp::Variable new_u = representative_map[dsu.find(u)];
        pcp::Variable new_v = representative_map[dsu.find(v)];
        new_BinaryCSP.add_constraint(new_u, new_v, c);
    }
    pcp = std::move(new_BinaryCSP);
}

namespace core {

#ifndef SINGLE_THREAD

namespace {

struct gap_amplification_task_result {
    std::vector<pcp::Variable> neighbors;
    pcp::BinaryCSP reduced_pcp;
};

}

pcp::BinaryCSP gap_amplification(pcp::BinaryCSP pcp, pcpp::TesterType tester_type) {
    to_expander(pcp, constants::EXPANDING_COEFFICIENT);
    pcp = reduce_degree(pcp, constants::DEGREE);

    unsigned int num_threads = std::thread::hardware_concurrency();
    if (num_threads == 0) num_threads = constants::SAFE_THREAD_NUMBER;

    size_t original_size = pcp.get_size();

    std::vector<pcp::BinaryCSP> reduced_pcps(original_size);

    std::vector<std::future<gap_amplification_task_result>> futures;
    futures.reserve(original_size);

    std::vector<std::vector<std::pair<pcp::Variable, size_t>>> occuring_location(original_size);

    util::thread_pool pool(num_threads);

    for (pcp::Variable u = 0; u < static_cast<pcp::Variable>(original_size); ++u) {
        futures.push_back(pool.enqueue([&pcp, tester_type, u]() {
            std::vector<pcp::Variable> neighbors = pcp.get_neighbors(u, constants::POWERING_RADIUS);
            pcp::BinaryCSP powering_u = pcp.build_sub_pcp(neighbors);

            std::unique_ptr<pcpp::Tester> tester = pcpp::get_tester(tester_type);
            tester->create_tester(powering_u);

            return gap_amplification_task_result{
                std::move(neighbors),
                tester->buildBinaryCSP()
            };
        }));
    }

    for (pcp::Variable u = 0; u < static_cast<pcp::Variable>(futures.size()); ++u) {
        gap_amplification_task_result task_result = futures[u].get();
        for (size_t i = 0; i < task_result.neighbors.size(); ++i) {
            occuring_location[task_result.neighbors[i]].emplace_back(u, i);
        }
        reduced_pcps[u] = std::move(task_result.reduced_pcp);
    }

    pcp = pcp::merge_BinaryCSPs(reduced_pcps);

    if (tester_type == pcpp::TesterType::HADAMARD) {
        // for Hadamard tester, we can further merge variables that are not merged in the tester but are actually the same due to the structure of the powering PCPs
        merge_variables(original_size, pcp, occuring_location, reduced_pcps);
    }

    pcp.clean();
    return pcp;
}

#else

pcp::BinaryCSP gap_amplification(pcp::BinaryCSP pcp, pcpp::TesterType tester_type) {
    to_expander(pcp, constants::EXPANDING_COEFFICIENT);
    pcp = reduce_degree(pcp, constants::DEGREE);
    size_t original_size = pcp.get_size();
    std::vector<std::vector<std::pair<pcp::Variable, size_t>>> occuring_location(original_size);

    std::vector<pcp::BinaryCSP> reduced_pcps;

    for (pcp::Variable u = 0; u < static_cast<pcp::Variable>(pcp.get_size()); ++u) {
        std::vector<pcp::Variable> neighbors = pcp.get_neighbors(u, constants::POWERING_RADIUS);
        for (size_t i = 0; i < neighbors.size(); ++i) {
            occuring_location[neighbors[i]].emplace_back(u, i);
        }
        pcp::BinaryCSP powering_u = pcp.build_sub_pcp(neighbors);
        std::unique_ptr<pcpp::Tester> tester = pcpp::get_tester(tester_type); tester->create_tester(powering_u);
        pcp::BinaryCSP reduced_pcp = tester->buildBinaryCSP();
        reduced_pcps.push_back(reduced_pcp);
    }
    pcp = pcp::merge_BinaryCSPs(reduced_pcps);

    merge_variables(original_size, pcp, occuring_location, reduced_pcps);
    pcp.clean();
    return pcp;
}

#endif

}