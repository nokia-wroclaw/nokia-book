#pragma once
#include "ir/ir.hpp"
#include "analysis/detecting_natural_loops.hpp"
#include "ir/symbol_table.hpp"
#include <unordered_map>

typedef std::unordered_map<Ir::Argument, int> SpillCosts;

SpillCosts computeSpillCosts(const std::set<Ir::Argument> &potentialSpills, const LocalStore &store, const Ir::Block &block, const NaturalLoopMapping &loopMapping);
