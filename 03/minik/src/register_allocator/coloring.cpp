#include "coloring.hpp"
#include "interference_graph.hpp"
#include "analysis/control_flow_graph.hpp"
#include "ir/ir_printer.hpp"
#include <stack>
#include <iostream>
#include "target_x86/x86_assembler.hpp"

ColoringResult linear_color(const Ir::Block &_block, size_t N)
{
    Ir::Block block = _block;
    LiveVariablesDataFlowAnalysis dfa_lv;
    ColoringResult coloringResult;
    std::unordered_map<Ir::Argument, int> &result = coloringResult.colors;

    std::stack<int> stack;

    auto cfg = buildControlFlowGraph(block);
    auto lv = dfa_lv.analyse(cfg);
    auto ig = buildInferenceGraph(lv, cfg);

    auto graph = ig.graph;

    for (auto v : graph) {
        auto r = graph.vertexData(v);
        if (r.is(Ir::ArgumentType::PinnedSpecialHardRegister))
            graph.removeVertex(v);
    }

    while (graph.getSize() > 0) {
        std::cerr << "SIZE "  << graph.getSize() << "\n";
        bool success = false;

        for (auto v : graph)
        if (graph.getVertexDegree(v) < N) {
            std::cerr << "VERTEX " << v << " degree " << graph.getVertexDegree(v) << "\n";
            std::cerr << "PUSH " << to_string(graph.vertexData(v)) << "\n";
            stack.push(v);
            graph.removeVertex(v);
            success = true;
            break;
        }

        if (not success)
        for (auto v : graph) {
            std::cerr << "POTENTIAL SPILL VERTEX " << v << " degree " << graph.getVertexDegree(v) << "\n";
            std::cerr << "PUSH " << to_string(graph.vertexData(v)) << "\n";
            stack.push(v);
            coloringResult.potentialSpills.insert(graph.vertexData(v));
            graph.removeVertex(v);
            break;
        }
    }

    while (not stack.empty()) {
        auto v = stack.top();
        stack.pop();

        auto isCompatible = [&] (int color)
        {
            for (auto u : ig.graph.getNeighbours(v)) {
                auto it = result.find(ig.graph.getVertexData(u));
                if (it != result.end()) {
                    if (it->second == color) return false;
                }
            }

            return true;
        };

        unsigned pickedColor;

        for (pickedColor = 0; pickedColor < N; pickedColor++) {
            if (isCompatible(pickedColor)) break;
        }
        if (pickedColor == N) {
            std::cerr << "ACTUAL SPILL vertex " << v << " reg " << to_string(ig.graph.vertexData(v)) << "\n";
            coloringResult.valid = false;
            return coloringResult;
        }

        std::cerr << "COLOR vertex " << v << " reg " << to_string(ig.graph.vertexData(v)) << " color " << pickedColor << "\n";
        result[ig.graph.getVertexData(v)] = pickedColor;
    }

    coloringResult.valid = true;
    return coloringResult;
}

Ir::Block applyColors(const std::unordered_map<Ir::Argument, int> &c, const Ir::Block &givenBlock)
{
    auto block = givenBlock;

    std::unordered_map<int , Ir::Argument> precolored;

    for (auto &p : c) {
        if (p.first.is(Ir::ArgumentType::PinnedHardRegister)) {
            precolored[p.second] = p.first;
        } else
        if (p.first.is(Ir::ArgumentType::AnyHardRegister)) {
            throw std::runtime_error("Register are already colorized");
        }
    }

    for (auto &p : c) {
        if (p.first.is(Ir::ArgumentType::TemporaryRegister)) {
            auto dst = Ir::Argument::AnyHardRegister(p.second);
            if (precolored.find(p.second) != precolored.end())
                dst = precolored.at(p.second);
            std::cerr << "map " << to_string(p.first) << " -> " << to_string(dst) << "\n";
            block = Ir::substituteRegister(block, p.first, dst);
        }
    }

    return block;
}
