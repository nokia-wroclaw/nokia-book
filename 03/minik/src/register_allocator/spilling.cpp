#include "spilling.hpp"
#include "ir/ir_printer.hpp"
#include "ir/ir.hpp"
#include <stdexcept>
#include <iostream>

Ir::Block insert_spillcode(LocalStore &localStore, const Ir::Block &block, const SpillCosts &costs) {

    if (costs.empty()) {
        throw std::runtime_error("cannot spill");
    }

    Ir::Argument selectedRegister = costs.begin()->first;
    int currentCost = costs.begin()->second;

    for (auto &p : costs) {
        if (p.second < currentCost) {
            currentCost = p.second;
            selectedRegister = p.first;
        }
    }

    std::cerr << "Selected register for spilling: " << to_string(selectedRegister) << "\n";
    int address = localStore.size();
    localStore[selectedRegister] = address;

    Ir::Block result = block;

    auto it = result.begin();
    while (it != result.end()) {
        std::cerr << "considering " << to_string(*it) << "\n";

        auto accessed = Ir::computeAccessedRegisters(*it);
        if (accessed.find(selectedRegister) != accessed.end()) {
            auto loadvar = Ir::Instruction{Ir::InstructionType::LoadVar, {selectedRegister, Ir::Argument::Integer(address)}, -1};
            result.insert(it, loadvar);
        }

        auto modified = Ir::computeModifiedRegisters(*it);
        if (modified.find(selectedRegister) != modified.end()) {
            auto storevar = Ir::Instruction{Ir::InstructionType::StoreVar, {selectedRegister, Ir::Argument::Integer(address)}, -1};
            ++it;
            result.insert(it, storevar);
        }

        ++it;
    }

    return result;
}
