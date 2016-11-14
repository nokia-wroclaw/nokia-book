#include "x86_assembler.hpp"
#include "utils/asm.hpp"
#include "ir/ir_printer.hpp"
#include <stdexcept>

namespace {

std::string getJumpCondition(Ir::InstructionType tp)
{
    switch (tp) {
        case Ir::InstructionType::IfEq: return "je";
        case Ir::InstructionType::IfNeq: return "jne";
        case Ir::InstructionType::IfLt: return "jl";
        case Ir::InstructionType::IfLeq: return "jle";
        case Ir::InstructionType::IfGt: return "jg";
        case Ir::InstructionType::IfGeq: return "jge";
    }
    return "<<invalid>>";
}

std::string getOpcode_Reg_Reg(Ir::InstructionType tp)
{
    switch (tp) {
        case Ir::InstructionType::Mov: return "movl";
        case Ir::InstructionType::Add: return "addl";
        case Ir::InstructionType::Sub: return "subl";
        case Ir::InstructionType::Mul: return "imull";
    }
    return "<<invalid>>";
}

std::string printReg(const Ir::Argument &arg)
{
    if (arg.is(Ir::ArgumentType::PinnedHardRegister)
        or arg.is(Ir::ArgumentType::PinnedSpecialHardRegister)) {
        switch (X86_Registers(arg.content)) {
            case X86_Registers::EAX: return "%eax";
            case X86_Registers::EBX: return "%ebx";
            case X86_Registers::ECX: return "%ecx";
            case X86_Registers::EDX: return "%edx";
            case X86_Registers::ESI: return "%esi";
            case X86_Registers::EDI: return "%edi";
            case X86_Registers::EBP: return "%ebp";
            case X86_Registers::ESP: return "%esp";
            default: throw std::runtime_error("invalid register: " + to_string(arg));
        }
    } else {
        return to_string(arg);
    }
}

void compile_instruction(const SymbolTable &symbolTable, Asm &out, const Ir::Instruction &instr)
{
    using namespace Ir;

    {
        out.createAndAddBlockWithoutLabel({ ""});
        std::string ret;
        ret += "# IR: ";
        ret += to_string(instr);
        out.createAndAddBlockWithoutLabel({ ret });
    }

    switch (instr.type) {
        case InstructionType::Call: {
            std::string ret;
            ret += "call ";
            ret += symbolTable.getFunction(instr.arguments[0]).symName;
            out.createAndAddBlockWithoutLabel({ ret });
            if (instr.arguments.size() > 2) {
                throw std::runtime_error("incompatible x86 IR: Call + params");
            }
        }
        break;

        case InstructionType::ReturnArg: {
            throw std::runtime_error("incompatible x86 IR: ReturnARg");
        }

        case InstructionType::Div: {
            throw std::runtime_error("incompatible x86 IR: Div");
        }

        case InstructionType::LoadVar: {
            throw std::runtime_error("incompatible x86 IR: LoadArg");
        }

        case InstructionType::Meta: {
            throw std::runtime_error("incompatible x86 IR: Meta");
        }

        case InstructionType::Return: {
            out.createAndAddBlockWithoutLabel({ "leave" });
            out.createAndAddBlockWithoutLabel({ "ret" });
        }
        break;

        case InstructionType::Label: {
            std::string ret;
            ret += "";
            ret += ".L";
            ret += std::to_string(instr.arguments[0].content);
            out.createAndAddBlockWithLabel({}, ret);
        }
        break;

        case InstructionType::IfEq:
        case InstructionType::IfNeq:
        case InstructionType::IfLt:
        case InstructionType::IfLeq:
        case InstructionType::IfGt:
        case InstructionType::IfGeq: {
            std::string ret;
            ret += "cmpl ";
            ret += printReg(instr.arguments[2]);
            ret += ", ";
            ret += printReg(instr.arguments[1]);


            std::string jmp;
            jmp += getJumpCondition(instr.type);
            jmp += " ";
            jmp += ".L";
            jmp += std::to_string(instr.arguments[0].content);
            out.createAndAddBlockWithoutLabel({ ret, jmp });
        }
        break;

        case InstructionType::Mov: 
        case InstructionType::Add: 
        case InstructionType::Sub: 
        case InstructionType::Mul: {
            std::string ret;
            ret += getOpcode_Reg_Reg(instr.type);
            ret += " ";
            ret += printReg(instr.arguments[1]);
            ret += ", ";
            ret += printReg(instr.arguments[0]);
            out.createAndAddBlockWithoutLabel({ ret });
        }
        break;

        case InstructionType::Push: {
            std::string ret;
            ret += "pushl ";
            ret += printReg(instr.arguments[0]);
            out.createAndAddBlockWithoutLabel({ ret });
        }
        break;

        case InstructionType::Pop: {
            std::string ret;
            ret += "popl ";
            ret += printReg(instr.arguments[0]);
            out.createAndAddBlockWithoutLabel({ ret });
        }
        break;

        case InstructionType::Load: {
            std::string ret;
            ret += "movl ";
            ret += to_string(instr.arguments[2]);
            ret += "(";
            ret += printReg(instr.arguments[1]);
            ret += "), ";
            ret += printReg(instr.arguments[0]);
            out.createAndAddBlockWithoutLabel({ ret });
        }
        break;

        case InstructionType::Store: {
            std::string ret;
            ret += "movl ";
            ret += printReg(instr.arguments[0]);
            ret += ", ";
            ret += to_string(instr.arguments[2]);
            ret += "(";
            ret += printReg(instr.arguments[1]);
            ret += ")";
            out.createAndAddBlockWithoutLabel({ ret });
        }
        break;

        case InstructionType::DivMod: {
            // TODO: sanity check if args are %eax %edx
            std::string ret;
            ret += "idiv ";
            ret += printReg(instr.arguments[2]);
            out.createAndAddBlockWithoutLabel({ ret });
        }
        break;

        case InstructionType::Goto: {
            std::string ret;
            ret += "jmp ";
            ret += ".";
            ret += to_string(instr.arguments[0]);
            out.createAndAddBlockWithoutLabel({ ret });
        }
        break;

        case InstructionType::AddConst: {
            std::string ret;
            ret += "addl $";
            ret += to_string(instr.arguments[1]);
            ret += ", ";
            ret += printReg(instr.arguments[0]);
            out.createAndAddBlockWithoutLabel({ ret });
        }
        break;

        case InstructionType::Const: {
            std::string ret;
            ret += "movl $";
            ret += to_string(instr.arguments[1]);
            ret += ", ";
            ret += printReg(instr.arguments[0]);
            out.createAndAddBlockWithoutLabel({ ret });
        }
        break;

        default: {
        }
        break;
    }
}

void compile_instructions(const SymbolTable &symbolTable, Asm &out, const Ir::Block &instructions)
{
    for (const auto &instruction : instructions) {
        compile_instruction(symbolTable, out, instruction);
    }
}

void compile_function(const SymbolTable &symbolTable, Asm &out, const FunctionDef &functionDef)
{
    out.createAndAddBlockWithoutLabel(
        { std::string("# function ") + functionDef.name + "/" + std::to_string(functionDef.arguments.size())
        });

    out.createAndAddBlockWithLabel({}, functionDef.symName);

    {
        out.createAndAddBlockWithoutLabel({ ""});
        std::string ret;

        ret += "# modified registers";

        for (auto r : functionDef.modifiedRegs) {
            ret += " ";
            ret += to_string(r);
        }

        out.createAndAddBlockWithoutLabel({ ret });
        ret = "";

        ret += "# frameSize ";
        auto frameSize = functionDef.localStore.size();
        ret += std::to_string(frameSize);
        out.createAndAddBlockWithoutLabel({ ret });

        out.createAndAddBlockWithoutLabel({ "pushl %ebp"});
        out.createAndAddBlockWithoutLabel({ "movl %esp, %ebp"});
        if (frameSize > 0) {
            ret = "subl $";
            ret += std::to_string(frameSize * 4);
            ret += ", %esp";
            out.createAndAddBlockWithoutLabel({ ret });
        }
    }

    compile_instructions(symbolTable, out, functionDef.body);

}

} // end of anonymous namespace

std::string Assembler::compile(const SymbolTable &symbolTable)
{
    Asm out;

    out.createAndAddBlockWithoutLabel(
            {".text", ".global __MINIK_main"});

    for (const auto &it: symbolTable.functionsMap) {
        if (not it.second.predefined) compile_function(symbolTable, out, it.second);
    }

    out.createAndAddBlockWithoutLabel(
            {".ident \"minik2 compiler by Michal Bartkowiak, Pawel Wieczorek @ Nokia\""}
    );

    return out.print();
}
