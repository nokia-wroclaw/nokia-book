#include "interference_graph.hpp"
#include "ir/ir.hpp"
#include "ir/ir_printer.hpp"
#include "target_x86/x86_assembler.hpp"
#include <iostream>

InferenceGraph buildInferenceGraph(const LiveVariablesDataFlowAnalysis::Result &lv, const ControlFlowGraph &cfg)
{
    using namespace Ir;
    InferenceGraph infg;
    std::set<Argument> regs;

    for (auto v : cfg.graph) {
        auto &ir = cfg.graph.getVertexData(v);
        for (auto r : computeModifiedRegisters(ir)) {
            if (not r.is(ArgumentType::PinnedSpecialHardRegister))
                regs.insert(r);
        }
        for (auto r : computeAccessedRegisters(ir)) {
            if (not r.is(ArgumentType::PinnedSpecialHardRegister))
                regs.insert(r);
        }
    }

    for (auto r : regs) {
        infg.regmap[r] = infg.graph.addVertex(r);
    }

    for (auto &r1 : regs)
    for (auto &r2 : regs)
    if (r1 != r2 and r1.isGeneralPurposeHardRegister() and r2.isGeneralPurposeHardRegister()) {
        std::cerr << "adding hard edge: " << to_string(r1) << " - " << to_string(r2) << "\n";
        infg.graph.addEdge(infg.regmap.at(r1), infg.regmap.at(r2));
    }

    for (auto v : cfg.graph) {
        auto &ir = cfg.graph.getVertexData(v);

        auto &lv_in = lv.entry.at(v);
        auto &lv_out = lv.exit.at(v);

        for (auto r : computeModifiedRegisters(ir)) {
            for (auto x : lv_out) {
                if (not x.is(ArgumentType::PinnedSpecialHardRegister) and not r.is(ArgumentType::PinnedSpecialHardRegister))
                if (x != r)
                    infg.graph.addEdge(infg.regmap.at(r), infg.regmap.at(x));
            }
        }
    }

    return infg;
}


void InferenceGraph::dump(std::ostream &os)
{
    os << "graph INFERENCE_GRAPH {\n";
    os << "layout=circo;\n";
    std::set<int> visited;

    for (auto v : graph) {
        std::string r = to_string(graph.getVertexData(v));
        os << "V" << v << "[label=\"" << r << "\"];\n";

        for (auto u : graph.getNeighbours(v)) {
            if (visited.find(u) == visited.end()) {
                os << "V" << v << " -- " << "V" << u << ";\n";
            }
        }

        visited.insert(v);
    }


    os << "}\n";
}

