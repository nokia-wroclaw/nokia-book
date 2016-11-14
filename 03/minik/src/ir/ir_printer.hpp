#pragma once

#include <string>
#include "ir.hpp"
#include "symbol_table.hpp"

std::string to_string_reg(int i);

std::string to_string_func(int i);

std::string to_string(const Ir::Argument &i);

std::string to_string(const Ir::InstructionType tp);

std::string to_string(const Ir::Instruction &instr);

std::string to_string(const Ir::Block &block);

std::string to_string(const FunctionDef &functionDef);

std::string to_string(const SymbolTable &st);
