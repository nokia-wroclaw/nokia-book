#include "spill_costs.hpp"
#include "ir/ir_printer.hpp"
#include <iostream>

namespace {

int power(int i)
{
    int n = 1;
    while (i > 0) {
        n *= 10;
        i--;
    }
    return n;
}


} // end of anonymous namespace

SpillCosts computeSpillCosts(const std::set<Ir::Argument> &potentialSpills, const LocalStore &store, const Ir::Block &block, const NaturalLoopMapping &loopMapping)
{
    SpillCosts costs;

    std::cerr << "Potential spills =";
    for (auto r : potentialSpills) {
        std::cerr << " ";
        std::cerr << to_string(r);
    }
    std::cerr << "\n";

    auto isSpillable = [&] (const Ir::Argument &r) {
        return r.is(Ir::ArgumentType::TemporaryRegister) and store.find(r) == store.end() and potentialSpills.find(r) != potentialSpills.end();
    };

    for (auto r : computeUsedRegisters(block)) {
        if (isSpillable(r)) {
            costs[r] = 0;
        }
    }

    for (auto &instr : block) {
        for (auto r : computeAccessedRegisters(instr))
        if (isSpillable(r)) {
            int depthFactor = power(loopMapping.at(instr.extra).size());;
            costs[r] += depthFactor;
        }
        for (auto r : computeModifiedRegisters(instr))
        if (isSpillable(r)) {
            int depthFactor = power(loopMapping.at(instr.extra).size());;
            costs[r] += depthFactor;
        }
    }

    return costs;
}
