#pragma once

#include "ir/ir.hpp"
#include "ir/symbol_table.hpp"

enum class X86_Registers
{
    EAX = 0,
    EBX = 1,
    ECX = 2,
    EDX = 3,
    ESI = 4,
    EDI = 5,
    EBP = 6,
    ESP = 7
};

struct X86_IrRegisters
{
    static constexpr Ir::Argument EAX = Ir::Argument::PinnedHardRegister(int(X86_Registers::EAX));
    static constexpr Ir::Argument EBX = Ir::Argument::PinnedHardRegister(int(X86_Registers::EBX));
    static constexpr Ir::Argument ECX = Ir::Argument::PinnedHardRegister(int(X86_Registers::ECX));
    static constexpr Ir::Argument EDX = Ir::Argument::PinnedHardRegister(int(X86_Registers::EDX));
    static constexpr Ir::Argument ESI = Ir::Argument::PinnedHardRegister(int(X86_Registers::ESI));
    static constexpr Ir::Argument EDI = Ir::Argument::PinnedHardRegister(int(X86_Registers::EDI));

    static constexpr Ir::Argument EBP = Ir::Argument::PinnedSpecialHardRegister(int(X86_Registers::EBP));
    static constexpr Ir::Argument ESP = Ir::Argument::PinnedSpecialHardRegister(int(X86_Registers::ESP));
};

class Assembler
{
public:
    std::string compile(const SymbolTable &symbolTable);
};
