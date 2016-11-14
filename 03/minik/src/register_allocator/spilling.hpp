#pragma once

#include "ir/ir.hpp"
#include "spill_costs.hpp"
#include "ir/symbol_table.hpp"

Ir::Block insert_spillcode(LocalStore &localStore, const Ir::Block &block, const SpillCosts &costs);
