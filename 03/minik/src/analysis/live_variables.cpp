#include "live_variables.hpp"
#include "ir/ir_printer.hpp"
#include "target_x86/x86_assembler.hpp"

LV_t LiveVariablesDataFlowAnalysisDriver::getExtremePoint(const ControlFlowGraph &cfg) const
{
    // x86 specific
    LV_t x;
    x.insert(Ir::Argument(X86_IrRegisters::EAX));
    return x;
}

LV_t LiveVariablesDataFlowAnalysisDriver::getEmpty(const ControlFlowGraph &cfg) const
{
    return LV_t{};
}

bool LiveVariablesDataFlowAnalysisDriver::isForwardAnalysis() const
{
    return false;
}

LV_t LiveVariablesDataFlowAnalysisDriver::transform(const Ir::Instruction &ir, const LV_t &x) const
{
    auto res = x;

    // kill
    for (auto i : Ir::computeModifiedRegisters(ir)) {
        res.erase(i);
    }

    // gen
    for (auto i : Ir::computeAccessedRegisters(ir)) {
        res.insert(i);
    }

    return res;
}

LV_t LiveVariablesDataFlowAnalysisDriver::join(const LV_t &x, const LV_t &y) const
{
    auto z = x;
    for (auto i : y) {
        z.insert(i);
    }

    return z;
}

bool LiveVariablesDataFlowAnalysisDriver::equal(const LV_t &x, const LV_t &y) const
{
    return x == y;
}


std::string LiveVariablesDataFlowAnalysisDriver::to_string(const LV_t &x)
{
    std::string ret;
    ret += "{ ";

    for (auto &i : x) {
        ret += ::to_string(i);
        ret += " ";
    }

    ret += "}";
    return ret;
}


void LiveVariablesDataFlowAnalysis::dump(std::ostream &os, const ControlFlowGraph &cfg, const Result &result)
{
    auto f_in = [&] (int v) {
        return to_string(result.entry[v]);
    };
    auto f_out = [&] (int v) {
        return to_string(result.exit[v]);
    };

    cfg.dumpWithIO(os, f_in, f_out);
}
