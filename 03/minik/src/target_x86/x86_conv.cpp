#include "analysis/control_flow_graph.hpp"
#include "analysis/live_variables.hpp"
#include "ir/ir_printer.hpp"
#include "x86_assembler.hpp"
#include "x86_conv.hpp"
#include "utils/headers.hpp"

#include <list>
#include <iostream>

namespace
{

Ir::Block rewrite_block(SymbolTable &st, const Ir::Block &_block)
{

    Ir::Block result = _block;

    step("live variables");

    auto cfg = buildControlFlowGraph(result);
    auto lv = LiveVariablesDataFlowAnalysis::analyse(cfg);
    LiveVariablesDataFlowAnalysis::dump(std::cerr, cfg, lv);

    auto it = result.begin();

    std::set<int> visited;

    step("input");
    std::cerr << to_string(result) << "\n";

    step("transformation");

    while (it != result.end()) {
        if (it->type == Ir::InstructionType::Call and visited.find(it->extra) == visited.end()) {
            auto v = it->extra;
            auto lv_in = lv.entry[v];
            const auto &fdef = st.getFunction(it->arguments[0]);
            visited.insert(v);

            auto orig_it = it;

            std::cerr << "found call: " << to_string(*it) << "\n";
            std::cerr << "live variables at entry: " << LiveVariablesDataFlowAnalysis::to_string(lv.entry.at(v)) << "\n";
            std::string ret;
            for (auto r : fdef.modifiedRegs) {
                ret += " ";
                ret += to_string(r);
            }
            std::cerr << "function modifies: " << ret << "\n";


            bool hasArguments = false;
            do {
                --it;
                hasArguments |= it->type == Ir::InstructionType::Push;
            } while (it->type == Ir::InstructionType::Push);
            ++it;

            std::list<Ir::Argument> saved;
            for (auto r : lv_in) {
                if (r.is(Ir::ArgumentType::PinnedHardRegister) and fdef.modifiedRegs.find(r) != fdef.modifiedRegs.end()) {
                    auto helper = Ir::Instruction{Ir::InstructionType::Push, {r}, 0};
                    result.insert(it, helper);
                    saved.push_front(r);
                }
            }

            if (saved.size() > 0) {
                it = orig_it;
                ++it;

                if (hasArguments) ++it;

                for (auto r : saved) {
                    auto helper = Ir::Instruction{Ir::InstructionType::Pop, {r}, 0};
                    result.insert(it, helper);
                }
            }

        }

        // TODO: temporary remove of Meta instructions CallerSave/CallerRestore
        if (it->type == Ir::InstructionType::Meta) {
            if (it->arguments[0].content == int(Ir::MetaTag::CallerSave)
                or it->arguments[0].content == int(Ir::MetaTag::CallerRestore)) {
                it = result.erase(it);
            }
        } else {
            ++it;
        }
    }

    step("output");
    std::cerr << to_string(result) << "\n";

    return result;
}

} // end of anonymous namespace

std::set<Ir::Argument> getAllGeneralPurposeHardRegisters()
{
    std::set<Ir::Argument> ir;

    for (int i = 0; i < 6; i++) {
        ir.insert(Ir::Argument::PinnedHardRegister(i));
    }

    return ir;
}

void x86_conv(SymbolTable &st)
{
    for (auto &p : st.functionsMap) {
        if (p.second.predefined) {
            p.second.modifiedRegs = getAllGeneralPurposeHardRegisters();
        } else {
            p.second.modifiedRegs = computeModifiedRegisters(p.second.body);
        }
    }

    for (auto &p : st.functionsMap) {
        if (not p.second.predefined) {
            subpass("fuction " + p.second.name);
            p.second.body = rewrite_block(st, p.second.body);
        }
    }
}

