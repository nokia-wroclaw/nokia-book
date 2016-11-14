#pragma once

#include "ir/ir.hpp"
#include <unordered_map>
#include <boost/optional.hpp>

struct ColoringResult
{
    bool valid;
    std::unordered_map<Ir::Argument, int> colors;
    std::set<Ir::Argument> potentialSpills;
};

ColoringResult linear_color(const Ir::Block &block, size_t N);
Ir::Block applyColors(const std::unordered_map<Ir::Argument, int> &c, const Ir::Block &block);
