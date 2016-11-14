#pragma once

#include "ir/symbol_table.hpp"

void x86_idiosyncrasy(SymbolTable &st);
void x86_store_operations(SymbolTable &st);
void x86_unfold_callreturn(SymbolTable &st);
void x86_default_return(SymbolTable &st);
