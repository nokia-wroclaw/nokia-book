#pragma once
#include <set>
#include "data_flow_analysis.hpp"
#include "ir/ir.hpp"

typedef std::set<Ir::Argument> LV_t;

struct LiveVariablesDataFlowAnalysisDriver: IDataFlowAnalysisDriver<LV_t>
{
    LV_t getExtremePoint(const ControlFlowGraph &cfg) const override;

    LV_t getEmpty(const ControlFlowGraph &cfg) const override;

    bool isForwardAnalysis() const override;

    LV_t transform(const Ir::Instruction &ir, const LV_t &x) const override;

    LV_t join(const LV_t &x, const LV_t &y) const override;

    bool equal(const LV_t &x, const LV_t &y) const override;
   
    static std::string to_string(const LV_t &x);
};

class LiveVariablesDataFlowAnalysis: public DataFlowAnalysis<LV_t>
{
public:
    LiveVariablesDataFlowAnalysis()
        : DataFlowAnalysis<LV_t>(new LiveVariablesDataFlowAnalysisDriver())
    {}

    static std::string to_string(const LV_t &x) {
        return LiveVariablesDataFlowAnalysisDriver::to_string(x);
    }

    static Result analyse(const ControlFlowGraph &cfg)
    {
        return LiveVariablesDataFlowAnalysis().DataFlowAnalysis<LV_t>::analyse(cfg);
    }
    
    static void dump(std::ostream &os, const ControlFlowGraph &cfg, const Result &result);
};
