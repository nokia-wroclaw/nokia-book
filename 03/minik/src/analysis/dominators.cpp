#include "dominators.hpp"
#include "ir/ir_printer.hpp"
#include <algorithm>
#include <iostream>

DOM_t DominatorsDataFlowAnalysisDriver::getExtremePoint(const ControlFlowGraph &cfg) const
{
    return DOM_t{};
}

DOM_t DominatorsDataFlowAnalysisDriver::getEmpty(const ControlFlowGraph &cfg) const
{
    auto res = cfg.graph.vertices();
    return res;
}

bool DominatorsDataFlowAnalysisDriver::isForwardAnalysis() const
{
    return true;
}

DOM_t DominatorsDataFlowAnalysisDriver::transform(const Ir::Instruction &ir, const DOM_t &x) const
{
    auto res = x;
    res.insert(ir.extra);
    return res;
}

DOM_t DominatorsDataFlowAnalysisDriver::join(const DOM_t &x, const DOM_t &y) const
{
    DOM_t z;
    std::set_intersection(x.begin(), x.end(), y.begin(), y.end(), std::inserter(z, z.begin()));
    return z;
}

bool DominatorsDataFlowAnalysisDriver::equal(const DOM_t &x, const DOM_t &y) const
{
    return x == y;
}

std::string DominatorsDataFlowAnalysisDriver::to_string(const DOM_t &x)
{
    std::string ret;
    ret += "{ ";

    for (auto &i : x) {
        ret += std::to_string(i);
        ret += " ";
    }

    ret += "}";
    return ret;
}


void DominatorsDataFlowAnalysis::dump(std::ostream &os, const ControlFlowGraph &cfg, const Result &result)
{
    auto f_in = [&] (int v) {
        return to_string(result.entry[v]);
    };
    auto f_out = [&] (int v) {
        return to_string(result.exit[v]);
    };

    cfg.dumpWithIO(os, f_in, f_out);
}
