#ifndef VISIT_GUARD_HPP
#define VISIT_GUARD_HPP

#include <vector>
#include "Aliases.hpp"

namespace util {

class visit_guard {
public:
    visit_guard(std::vector<bool> &visited, const std::vector<pcp::Variable> &nodes);
    
    ~visit_guard();

private:
    std::vector<bool> &visited;
    const std::vector<pcp::Variable> &nodes;
};

}

#endif