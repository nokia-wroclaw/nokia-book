#pragma once

#include "utils/graph.hpp"
#include "ir/ir.hpp"
#include <sstream>
#include <functional>


struct ControlFlowGraph {
    typedef DirectedGraph<Ir::Instruction>           Graph;
    typedef DirectedGraphDirection<Ir::Instruction>   Direction;
    typedef DirectedGraphForwardDirection<Ir::Instruction>   ForwardDirection;
    typedef DirectedGraphBackwardDirection<Ir::Instruction>  BackwardDirection;


    ControlFlowGraph();

    Graph graph;
    Graph::V_t isolatedEntry;
    Graph::V_t isolatedExit;

    void dump(std::ostream &os, bool printIr) const;

    void dumpWithIO(std::ostream &os, std::function<std::string (int)> inF,  std::function<std::string (int)> outF) const;
};

ControlFlowGraph buildControlFlowGraph(Ir::Block &block);

