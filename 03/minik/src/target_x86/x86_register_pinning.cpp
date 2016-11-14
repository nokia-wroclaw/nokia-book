#include "x86_register_pinning.hpp"
#include "ir/symbol_table.hpp"
#include "x86_assembler.hpp"
#include "utils/headers.hpp"
#include "ir/ir_printer.hpp"

#include <set>
#include <unordered_map>
#include <algorithm>
#include <iostream>

namespace {

const std::set<Ir::Argument> regs = {
    X86_IrRegisters::EAX,
    X86_IrRegisters::EBX,
    X86_IrRegisters::ECX,
    X86_IrRegisters::EDX,
    X86_IrRegisters::ESI,
    X86_IrRegisters::EDI
};

std::set<Ir::Argument> getFreeHwRegisters(const Ir::Block &block) {
    // all regs that can be used
    // determine which hard registers are already pinned
    std::set<Ir::Argument> pinnedRegs;
    for (const auto &ir: block)
        for (const auto &arg: ir.arguments)
            if (arg.is(Ir::ArgumentType::PinnedHardRegister))
                pinnedRegs.insert(arg);

    for (const auto &pinned: pinnedRegs)
        std::cerr << "pre-prinned: " << to_string(pinned) << "\n";

    // subtract the pinned registers from all available
    std::set<Ir::Argument> result;
    std::set_difference(
            regs.cbegin(), regs.cend(),
            pinnedRegs.cbegin(), pinnedRegs.cend(),
            std::inserter(result, result.end()));

    // return the result
    return result;
}

void x86_pin_for_func(FunctionDef &func) {
    auto &body = func.body;
    auto freeHwRegisters = getFreeHwRegisters(body);

    std::unordered_map<Ir::Argument, Ir::Argument> pinnedRegisters;
    for (auto &instr: body) {
        for (auto &arg: instr.arguments) {
            if (arg.is(Ir::ArgumentType::AnyHardRegister)) {
                auto candidate = pinnedRegisters.find(arg);
                if (pinnedRegisters.end() != candidate) {
                    arg = candidate->second;
                } else {
                    std::cerr << "trying to pin " << to_string(arg) << "\n";
                    // allocate new hw reg
                    auto newReg = *(freeHwRegisters.begin());
                    freeHwRegisters.erase(freeHwRegisters.begin());
                    pinnedRegisters[arg] = newReg;

                    std::cerr << "pin: " << to_string(arg) << " -> " << to_string(newReg) << "\n";
                    arg = newReg;
                }
            }
        }
    }
}

} // namespace


void x86_pin(SymbolTable &st) {
    for (auto &func : st.functionsMap) {
        subpass("function " + func.second.name);
        x86_pin_for_func(func.second);
    }
}
