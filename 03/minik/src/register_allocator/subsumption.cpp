#include "subsumption.hpp"
#include "analysis/control_flow_graph.hpp"
#include "interference_graph.hpp"
#include "analysis/live_variables.hpp"
#include "ir/ir.hpp"
#include "ir/ir_printer.hpp"

#include <iostream>

Ir::Block subsumption(Ir::Block block)
{
    bool restart = false;
    LiveVariablesDataFlowAnalysis dfa_lv;

    auto iter = [&] (const InferenceGraph &ig) {

        for (auto ir : block) {
            if (ir.type != Ir::InstructionType::Mov) continue;

            std::cerr << "checking move: " <<  to_string(ir) << "\n";

            auto from = ir.arguments[0];
            auto to = ir.arguments[1];
            int u = ig.regmap.at(ir.arguments[0]);
            int v = ig.regmap.at(ir.arguments[1]);

            if (u != v and not ig.graph.hasEdge(v, u)) {

                if (not from.is(Ir::ArgumentType::TemporaryRegister)) {
                    std::swap(to, from);
                }
                if (not from.is(Ir::ArgumentType::TemporaryRegister)) continue;

                std::cerr << "subsumption " << to_string(from) << " -> " << to_string(to) << "\n";
                block = Ir::cleanup(Ir::substituteRegister(block, from, to));
                restart = true;
                return;
            }
        }
    };

    do {
        restart = false;
        auto cfg = buildControlFlowGraph(block);
        auto result = dfa_lv.analyse(cfg);
        auto ig = buildInferenceGraph(result, cfg);
        iter(ig);
    } while(restart);

    return block;
}
