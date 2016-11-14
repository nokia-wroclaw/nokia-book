#pragma once

#include "analysis/control_flow_graph.hpp"
#include "analysis/live_variables.hpp"
#include "utils/graph.hpp"
#include "ir/ir.hpp"
#include <unordered_map>
#include <sstream>

struct InferenceGraph
{
    typedef UndirectedGraph<Ir::Argument>   Graph;

    Graph   graph;
    std::unordered_map<Ir::Argument, InferenceGraph::Graph::V_t> regmap;

    void dump(std::ostream &os);

    void coalescence(InferenceGraph::Graph::V_t a, InferenceGraph::Graph::V_t b);
};

InferenceGraph buildInferenceGraph(const LiveVariablesDataFlowAnalysis::Result &lv, const ControlFlowGraph &graph);
