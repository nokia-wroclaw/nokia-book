#include "detecting_natural_loops.hpp"
#include <stack>
#include <iostream>

NaturalLoopMapping detectNaturalLoop(const ControlFlowGraph &cfg, const DominatorsDataFlowAnalysis::Result &result)
{
    NaturalLoopMapping mapping;

    std::stack<std::pair<int, int>> backedges;

    for (auto v : cfg.graph) {
        mapping[v].clear();
        auto successors = cfg.graph.getOutNeighbours(v);
        auto dominators = result.exit.at(v);

        for (auto u : successors)
        if (dominators.find(u) != dominators.end()) {
            std::cerr << "detected back edge " << v << " -> " << u << "\n";
            backedges.push(std::make_pair(v, u));
        }
    }

    while (not backedges.empty()) {
        auto backedge = backedges.top();
        backedges.pop();

        auto header = backedge.second;
        std::set<int> body;
        std::stack<int> todo;
        body.insert(header);
        todo.push(backedge.first);

        while (not todo.empty()) {
            auto v = todo.top();
            todo.pop();
            body.insert(v);
            mapping[v].insert(header);

            for (auto u : cfg.graph.getInNeighbours(v))
            if (body.find(u) == body.end()) {
                todo.push(u);
            }
        }
    }

    return mapping;
}
