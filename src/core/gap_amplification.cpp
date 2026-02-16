#include <cmath>
#include <future>
#include <vector>
#include <mutex>

#include "core/core.hpp"
#include "constants.hpp"
#include "pcpp/TesterFactory.hpp"
#include "util/disjoint_set_union.hpp"

#include <cassert>

void merge_variables(
    size_t original_size,
    pcp::BitPCP &pcp, 
    const std::vector<std::vector<std::pair<pcp::Variable, size_t>>> &occuring_location,
    const std::vector<pcp::BitPCP> &reduced_pcps
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

    pcp::BitPCP new_bitpcp(new_size);

    for (pcp::Variable i = 0; i < offsets.back(); ++i) {
        if (dsu.find(i) == i) {
            new_bitpcp.set_variable(
                representative_map[i],
                pcp.get_variable(i)
            );
        }
    }

    for (const auto &[u, v, c] : pcp.get_constraints_list()) {
        pcp::Variable new_u = representative_map[dsu.find(u)];
        pcp::Variable new_v = representative_map[dsu.find(v)];
        new_bitpcp.add_constraint(new_u, new_v, c);
    }
    pcp = std::move(new_bitpcp);
}

namespace core {

#ifndef SINGLE_THREAD

pcp::BitPCP gap_amplification(pcp::BitPCP pcp, pcpp::TesterType tester_type) {
    to_expander(pcp, constants::EXPANDING_COEFFICIENT);
    pcp = reduce_degree(pcp, constants::DEGREE);

    unsigned int num_threads = std::thread::hardware_concurrency();
    if (num_threads == 0) num_threads = constants::SAFE_THREAD_NUMBER;

    size_t original_size = pcp.get_size();

    std::vector<pcp::BitPCP> reduced_pcps(original_size);

    std::vector<std::future<void>> futures;
    size_t chunk_size = (original_size + num_threads - 1) / num_threads;

    std::vector<std::vector<std::pair<pcp::Variable, size_t>>> occuring_location(original_size);
    std::mutex mtx;

    for (unsigned int t = 0; t < num_threads; ++t) {
        size_t start = t * chunk_size;
        size_t end = std::min(start + chunk_size, original_size);

        if (start >= original_size) break;

        futures.push_back(std::async(std::launch::async,
            [&reduced_pcps, start, end, &pcp, &occuring_location, &mtx, tester_type]() {
                for (size_t u = start; u < end; ++u) {
                    std::vector<pcp::Variable> neighbors = pcp.get_neighbors(u, constants::POWERING_RADIUS);
                    {
                        std::lock_guard<std::mutex> lock(mtx);
                        for (size_t i = 0; i < neighbors.size(); ++i) {
                            occuring_location[neighbors[i]].emplace_back(u, i);
                        }
                    }
                    pcp::BitPCP powering_u = pcp.build_sub_pcp(neighbors);

                    std::unique_ptr<pcpp::Tester> tester = pcpp::get_tester(tester_type); tester->create_tester(powering_u);
                    pcp::BitPCP reduced_pcp = tester->buildBitPCP();
                    reduced_pcps[u] = std::move(reduced_pcp);
                }
            }
        ));
    }
    for (auto &f : futures) {
        f.get();
    }
    pcp = pcp::merge_BitPCPs(reduced_pcps);

    if (tester_type == pcpp::TesterType::HADAMARD) {
        // for Hadamard tester, we can further merge variables that are not merged in the tester but are actually the same due to the structure of the powering PCPs
        merge_variables(original_size, pcp, occuring_location, reduced_pcps);
    }

    pcp.clean();
    return pcp;
}

#else

pcp::BitPCP gap_amplification(pcp::BitPCP pcp, pcpp::TesterType tester_type) {
    to_expander(pcp, constants::EXPANDING_COEFFICIENT);
    pcp = reduce_degree(pcp, constants::DEGREE);
    size_t original_size = pcp.get_size();
    std::vector<std::vector<std::pair<pcp::Variable, size_t>>> occuring_location(original_size);

    std::vector<pcp::BitPCP> reduced_pcps;

    for (pcp::Variable u = 0; u < static_cast<pcp::Variable>(pcp.get_size()); ++u) {
        std::vector<pcp::Variable> neighbors = pcp.get_neighbors(u, constants::POWERING_RADIUS);
        for (size_t i = 0; i < neighbors.size(); ++i) {
            occuring_location[neighbors[i]].emplace_back(u, i);
        }
        pcp::BitPCP powering_u = pcp.build_sub_pcp(neighbors);
        std::unique_ptr<pcpp::Tester> tester = pcpp::get_tester(tester_type); tester->create_tester(powering_u);
        pcp::BitPCP reduced_pcp = tester->buildBitPCP();
        reduced_pcps.push_back(reduced_pcp);
    }
    pcp = pcp::merge_BitPCPs(reduced_pcps);

    merge_variables(original_size, pcp, occuring_location, reduced_pcps);
    pcp.clean();
    return pcp;
}

#endif

}