#pragma once

#include "control_flow_graph.hpp"
#include "ir/ir.hpp"
#include <memory>

template <typename L>
struct IDataFlowAnalysisDriver {
    virtual L getExtremePoint(const ControlFlowGraph &cfg) const = 0;

    virtual L getEmpty(const ControlFlowGraph &cfg) const = 0;

    virtual bool isForwardAnalysis() const = 0;

    virtual L transform(const Ir::Instruction &ir, const L &x) const = 0;

    virtual L join(const L &x, const L &y) const = 0;

    virtual bool equal(const L &x, const L &y) const = 0;
};


template <typename L>
class DataFlowAnalysis
{
public:
    typedef std::vector<L> ResultTable;

    struct IntermediateResult {
        IntermediateResult(const L &x, std::size_t n)
            : table({ ResultTable(n, x), ResultTable{n, x} })
        {}

        ResultTable table[2];

        static const unsigned int INPUT = 0;

        static const unsigned int OUTPUT = 1;


        L &input(size_t index) {
            return table[INPUT][index];
        }

        L &output(size_t index) {
            return table[OUTPUT][index];
        }

    };

    struct Result {
        ResultTable entry;
        ResultTable exit;

        Result(IntermediateResult &ir, bool forward) {
            auto entry_index = IntermediateResult::INPUT;
            auto exit_index = IntermediateResult::OUTPUT;

            if (not forward) std::swap(entry_index, exit_index);

            entry.swap(ir.table[entry_index]);
            exit.swap(ir.table[exit_index]);
        }
    };

    DataFlowAnalysis(IDataFlowAnalysisDriver<L> *driverPtr)
        : driver(driverPtr)
        , graphDirection(createGraphDirection(driverPtr)) {

    }

    struct Computer
    {
        Computer(const ControlFlowGraph &cfg, const IDataFlowAnalysisDriver<L> &driver, const ControlFlowGraph::Direction &graphDirection)
            : cfg(cfg)
            , driver(driver)
            , graphDirection(graphDirection)
            , result(driver.getEmpty(cfg), cfg.graph.getMaxIndex())
        {}

        Result compute() {
            while (performStep());

            return Result(result, driver.isForwardAnalysis());
        }

        L computeNodeInput(ControlFlowGraph::Graph::V_t v) {
            auto &predecessors = graphDirection.getPredecessors(cfg.graph, v);

            if (predecessors.empty()) return driver.getExtremePoint(cfg);

            L x = driver.getEmpty(cfg);
            for (auto u : predecessors) {
                x = driver.join(x, result.output(u));
            }

            return x;
        }

        bool performStep() {
            bool changed = false;

            for (auto v : cfg.graph) {
                result.input(v) = computeNodeInput(v);
                auto y = driver.transform(cfg.graph.getVertexData(v), result.input(v));
            
                if (not driver.equal(y, result.output(v))) {
                    result.output(v) = y;
                    changed = true;
                }
            }
            return changed;
        }


        const ControlFlowGraph &cfg;
        const IDataFlowAnalysisDriver<L> &driver;
        const ControlFlowGraph::Direction &graphDirection;
        IntermediateResult result;
    };

    Result analyse(const ControlFlowGraph &cfg) const {
        return Computer(cfg, *driver, *graphDirection).compute();
    }

private:

    static ControlFlowGraph::Direction *createGraphDirection(IDataFlowAnalysisDriver<L> *driverPtr) {
        if (driverPtr->isForwardAnalysis()) return new ControlFlowGraph::ForwardDirection();
        return new ControlFlowGraph::BackwardDirection();
    }

    std::unique_ptr<IDataFlowAnalysisDriver<L>>  driver;
    std::unique_ptr<ControlFlowGraph::Direction> graphDirection;

};
