#pragma once
#include <set>
#include "data_flow_analysis.hpp"
#include "ir/ir.hpp"

typedef std::set<unsigned int> DOM_t;

struct DominatorsDataFlowAnalysisDriver: IDataFlowAnalysisDriver<DOM_t>
{
    DOM_t getExtremePoint(const ControlFlowGraph &cfg) const override;

    DOM_t getEmpty(const ControlFlowGraph &cfg) const override;

    bool isForwardAnalysis() const override;

    DOM_t transform(const Ir::Instruction &ir, const DOM_t &x) const override;

    DOM_t join(const DOM_t &x, const DOM_t &y) const override;

    bool equal(const DOM_t &x, const DOM_t &y) const override;
   
    static std::string to_string(const DOM_t &x);
};

class DominatorsDataFlowAnalysis: public DataFlowAnalysis<DOM_t>
{
public:
    DominatorsDataFlowAnalysis()
        : DataFlowAnalysis<DOM_t>(new DominatorsDataFlowAnalysisDriver())
    {}

    static std::string to_string(const DOM_t &x) {
        return DominatorsDataFlowAnalysisDriver::to_string(x);
    }

    static Result analyse(const ControlFlowGraph &cfg)
    {
        return DominatorsDataFlowAnalysis().DataFlowAnalysis<DOM_t>::analyse(cfg);
    }
    
    static void dump(std::ostream &os, const ControlFlowGraph &cfg, const Result &result);
};
