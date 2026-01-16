#include <cmath>
#include <future>
#include <vector>
#include <mutex>

#include "core/core.hpp"
#include "constants.hpp"
#include "pcpp/Tester.hpp"

void add_consistency_constraints(
    size_t total_size,
    pcp::BitPCP &pcp, 
    const std::vector<std::vector<std::pair<pcp::Variable, size_t>>> &occuring_location,
    const std::vector<pcp::BitPCP> &reduced_pcps
) {
    
    std::vector<size_t> offsets(1, 0);
    offsets.reserve(total_size + 1);
    for (const auto &reduced_pcp : reduced_pcps) {
        offsets.push_back(offsets.back() + reduced_pcp.get_size());
    }

    for (pcp::Variable v = 0; v < static_cast<pcp::Variable>(total_size); ++v) {
        if (occuring_location[v].size() <= 1) continue;
        const auto &locations = occuring_location[v];
        for (size_t i = 1; i < locations.size(); ++i) {
            pcp::Variable u1 = locations[i - 1].first;
            size_t pos1 = locations[i - 1].second;
            pcp::Variable u2 = locations[i].first;
            size_t pos2 = locations[i].second;
            pcp.add_constraint(
                offsets[u1] + pos1 * 3,
                offsets[u2] + pos2 * 3,
                constraint::BitConstraint::EQUAL
            );
            pcp.add_constraint(
                offsets[u1] + pos1 * 3 + 1,
                offsets[u2] + pos2 * 3 + 1,
                constraint::BitConstraint::EQUAL
            );
            pcp.add_constraint(
                offsets[u1] + pos1 * 3 + 2,
                offsets[u2] + pos2 * 3 + 2,
                constraint::BitConstraint::EQUAL
            );
        }
    }
}

namespace core {

#ifndef SINGLE_THREAD

pcp::BitPCP gap_amplification(pcp::BitPCP pcp) {
    to_expander(pcp, constants::EXPANDING_COEFFICIENT);
    pcp = reduce_degree(pcp, constants::DEGREE);

    unsigned int num_threads = std::thread::hardware_concurrency();
    if (num_threads == 0) num_threads = constants::SAFE_THREAD_NUMBER;

    size_t total_size = pcp.get_size();

    std::vector<pcp::BitPCP> reduced_pcps(total_size);

    std::vector<std::future<void>> futures;
    size_t chunk_size = (total_size + num_threads - 1) / num_threads;

    std::vector<std::vector<std::pair<pcp::Variable, size_t>>> occuring_location(total_size);
    std::mutex mtx;

    for (unsigned int t = 0; t < num_threads; ++t) {
        size_t start = t * chunk_size;
        size_t end = std::min(start + chunk_size, total_size);

        if (start >= total_size) break;

        futures.push_back(std::async(std::launch::async,
            [&reduced_pcps, start, end, &pcp, &occuring_location, &mtx]() {
                for (size_t u = start; u < end; ++u) {
                    std::vector<pcp::Variable> neighbors = pcp.get_neighbors(u, constants::POWERING_RADIUS);
#ifdef ENFORCE_CONSISTENCY
                    std::lock_guard<std::mutex> lock(mtx);
                    for (size_t i = 0; i < neighbors.size(); ++i) {
                        occuring_location[neighbors[i]].emplace_back(u, i);
                    }
#endif
                    pcp::BitPCP powering_u = pcp.build_sub_pcp(neighbors);
                    pcp::BitPCP reduced_pcp = pcpp::Tester(powering_u).buildBitPCP();
                    reduced_pcps[u] = std::move(reduced_pcp);
                }
            }
        ));
    }
    for (auto &f : futures) {
        f.get();
    }
    pcp = pcp::merge_BitPCPs(reduced_pcps);

#ifdef ENFORCE_CONSISTENCY
    add_consistency_constraints(total_size, pcp, occuring_location, reduced_pcps);
#endif

    return pcp;
}

#else

pcp::BitPCP gap_amplification(pcp::BitPCP pcp) {
    to_expander(pcp, constants::EXPANDING_COEFFICIENT);
    pcp = reduce_degree(pcp, constants::DEGREE);

    std::vector<pcp::BitPCP> reduced_pcps;

    for (pcp::Variable u = 0; u < static_cast<pcp::Variable>(pcp.get_size()); ++u) {
        std::vector<pcp::Variable> neighbors = pcp.get_neighbors(u, constants::POWERING_RADIUS);
#ifdef ENFORCE_CONSISTENCY
        for (size_t i = 0; i < neighbors.size(); ++i) {
            occuring_location[neighbors[i]].emplace_back(u, i);
        }
#endif
        pcp::BitPCP powering_u = pcp.build_sub_pcp(neighbors);
        pcp::BitPCP reduced_pcp = pcpp::Tester(powering_u).buildBitPCP();
        reduced_pcp.clean();
        reduced_pcps.push_back(reduced_pcp);
    }
    pcp = pcp::merge_BitPCPs(reduced_pcps);

#ifdef ENFORCE_CONSISTENCY
    add_consistency_constraints(pcp, occuring_location, reduced_pcps);
#endif

    return pcp;
}

#endif

}