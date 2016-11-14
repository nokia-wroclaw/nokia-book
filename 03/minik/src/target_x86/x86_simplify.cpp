#include "x86_simplify.hpp"
#include "x86_assembler.hpp"
#include "ir/ir_printer.hpp"
#include <functional>
#include <iostream>

namespace
{

void rewrite_idiosyncrasy(const FunctionDef &def, Ir::Block &ret, Ir::Instruction instr)
{
    using namespace Ir;

    switch (instr.type) {
        case InstructionType::Div: {
            ret.push_back({InstructionType::Mov, { X86_IrRegisters::EAX, instr.arguments[0]}, 0});
            ret.push_back({InstructionType::Const, { X86_IrRegisters::EDX, Argument::Integer(0)}, 0});
            ret.push_back({InstructionType::DivMod, { X86_IrRegisters::EAX, X86_IrRegisters::EDX, instr.arguments[1]}, 0});
            ret.push_back({InstructionType::Mov, { instr.arguments[0], X86_IrRegisters::EAX}, 0});
            break;
        }

        case InstructionType::Call: {
            auto orig = instr.arguments[1];
            instr.arguments[1] = X86_IrRegisters::EAX;
            ret.push_back(instr);
            ret.push_back({InstructionType::Mov, {orig, X86_IrRegisters::EAX}, 0});
            break;
        }

        case InstructionType::ReturnArg: {
            ret.push_back({InstructionType::Mov, { X86_IrRegisters::EAX, instr.arguments[0]}, 0});
            ret.push_back({InstructionType::Return,{}, 0});
            break;
        }

        default:
            ret.push_back(instr);
            break;
    }
}

void rewrite_callreturn(const FunctionDef &def, Ir::Block &ret, Ir::Instruction instr)
{
    using namespace Ir;

    switch (instr.type) {
        case InstructionType::ReturnArg: {
            ret.push_back({InstructionType::Mov, { X86_IrRegisters::EAX, instr.arguments[0]}, 0});
            ret.push_back({InstructionType::Return,{}, 0});
            break;
        }

        case InstructionType::Call: {
            size_t args = instr.arguments.size() - 2;
            for (size_t i = 0; i < args; i++) {
                ret.push_back({InstructionType::Push, {instr.arguments[args - i + 2 - 1]}, 0});
            }
            instr.arguments.resize(2);

            ret.push_back(instr);
            if (args > 0) {
                ret.push_back({InstructionType::AddConst, {X86_IrRegisters::ESP, Argument::Integer((args)*4)}, 0});
            }
            break;
        }

        default:
            ret.push_back(instr);
            break;
    }
}

void rewrite_store(const FunctionDef &def, Ir::Block &ret, Ir::Instruction instr)
{
    using namespace Ir;

    auto getAddress = [&] (int c) {
        if (c < 0) {
            c *= -1;
            return Argument::Integer(4 * c + 4);
        }
        return Argument::Integer(-4 * c - 4);
    };

    switch (instr.type) {
        case InstructionType::LoadVar:
            instr.type = InstructionType::Load;
            instr.arguments.push_back(getAddress(instr.arguments[1].content));
            instr.arguments[1] = X86_IrRegisters::EBP;
            ret.push_back(instr);
            break;

        case InstructionType::StoreVar:
            instr.type = InstructionType::Store;
            instr.arguments.push_back(getAddress(instr.arguments[1].content));
            instr.arguments[1] = X86_IrRegisters::EBP;
            ret.push_back(instr);
            break;

        default:
            ret.push_back(instr);
            break;
    }
}

void rewrite_body(FunctionDef &def, std::function<void (const FunctionDef &def, Ir::Block &ret, Ir::Instruction instr)> rewriter)
{
    using namespace Ir;
    Block ret;

    for (auto instr : def.body) {
        rewriter(def, ret, instr);
    }

    def.body.swap(ret);
}

void appendDefaultReturn(FunctionDef &def)
{
    using namespace Ir;

    if (def.body.empty()
        or (InstructionType::Return != def.body.back().type
            and InstructionType::Goto != def.body.back().type)) {
        def.body.push_back(
            Instruction{InstructionType::Return, Arguments{}, 0});
    }
}

} // end of anonymous namespace


void x86_idiosyncrasy(SymbolTable &st)
{
    for (auto &p : st.functionsMap)
        if (not p.second.predefined) {
            rewrite_body(p.second, rewrite_idiosyncrasy);
        }

    std::cerr << to_string(st) << "\n";
}

void x86_store_operations(SymbolTable &st)
{
    for (auto &p : st.functionsMap)
        if (not p.second.predefined) {
            rewrite_body(p.second, rewrite_store);
        }

    std::cerr << to_string(st) << "\n";
}

void x86_unfold_callreturn(SymbolTable &st)
{
    for (auto &p : st.functionsMap)
        if (not p.second.predefined) {
            rewrite_body(p.second, rewrite_callreturn);
        }

    std::cerr << to_string(st) << "\n";
}

void x86_default_return(SymbolTable &st)
{
    for (auto &p : st.functionsMap)
        if (not p.second.predefined)
            appendDefaultReturn(p.second);

    std::cerr << to_string(st) << "\n";
}

