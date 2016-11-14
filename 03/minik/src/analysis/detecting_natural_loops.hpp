#pragma once

#include "dominators.hpp"
#include "control_flow_graph.hpp"

#include <unordered_map>
#include <set>

typedef std::unordered_map<int, std::set<int>> NaturalLoopMapping;

NaturalLoopMapping detectNaturalLoop(const ControlFlowGraph &cfg, const DominatorsDataFlowAnalysis::Result &result);
