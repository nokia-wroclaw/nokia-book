#include "ir_printer.hpp"

const char *getPrefixForArgumentType(const Ir::ArgumentType tp)
{
    switch (tp) {
    case Ir::ArgumentType::FunctionNumber: return "FUNC_";
    case Ir::ArgumentType::AnyHardRegister: return "%h";
    case Ir::ArgumentType::PinnedHardRegister: return "%H";
    case Ir::ArgumentType::PinnedSpecialHardRegister: return "%S";
    case Ir::ArgumentType::TemporaryRegister: return "%t";
    case Ir::ArgumentType::Integer: return "";
    case Ir::ArgumentType::LabelNumber: return "L";
    case Ir::ArgumentType::Meta: return "";
    default: break;
    }

    return "<<invalid>>";
}

std::string to_string_meta(int tag)
{
    using namespace Ir;
    switch (Ir::MetaTag(tag)) {
        case MetaTag::Exit:
            return "exit";
        case MetaTag::Entry:
            return "entry";
        case MetaTag::External:
            return "external";
        case MetaTag::CallerSave:
            return "caller save";
        case MetaTag::CallerRestore:
            return "caller restore";
    }

    return "invalid";
}

std::string to_string(const Ir::Argument &i)
{
    std::string ret;
    if (i.type == Ir::ArgumentType::Meta) {
        return to_string_meta(i.content);
    }
    ret += getPrefixForArgumentType(i.type);
    ret += std::to_string(i.content);
    return ret;
}

std::string to_string_label(int i)
{
    std::string ret;

    ret += "L";
    ret += std::to_string(i);

    return ret;
}

std::string to_string_func(int i)
{
    std::string ret;

    ret += "FUN_";
    ret += std::to_string(i);

    return ret;
}

std::string to_string(const Ir::InstructionType tp)
{
    using namespace Ir;
    std::string ret;

    switch (tp) {
        case InstructionType::Call: {
            ret += "call";
            break;
        }

        case InstructionType::ReturnArg: {
            ret += "return arg";
            break;
        }

        case InstructionType::Return: {
            ret += "return";
            break;
        }

        case InstructionType::Goto: {
            ret += "goto";
            break;
        }

        case InstructionType::IfEq: {
            ret += "if eq";
            break;
        }

        case InstructionType::IfNeq: {
            ret += "if neq";
            break;
        }

        case InstructionType::IfLt: {
            ret += "if lt";
            break;
        }

        case InstructionType::IfLeq: {
            ret += "if leq";
            break;
        }

        case InstructionType::IfGt: {
            ret += "if gt";
            break;
        }

        case InstructionType::IfGeq: {
            ret +="if geq";
            break;
        }

        case InstructionType::Add: {
            ret += "add";
            break;
        }

        case InstructionType::AddConst: {
            ret += "add";
            break;
        }

        case InstructionType::Sub: {
            ret += "sub";
            break;
        }

        case InstructionType::Mul: {
            ret += "mul";
            break;
        }

        case InstructionType::Div: {
            ret += "div";
            break;
        }

        case InstructionType::DivMod: {
            ret += "divmod";
            break;
        }

        case InstructionType::Mov: {
            ret += "mov";
            break;
        }

        case InstructionType::Load: {
            ret += "load";
            break;
        }

        case InstructionType::Store: {
            ret += "store";
            break;
        }

        case InstructionType::StoreVar: {
            ret += "store var";
            break;
        }

        case InstructionType::LoadVar: {
            ret += "load var";
            break;
        }

        case InstructionType::Push: {
            ret += "push";
            break;
        }

        case InstructionType::Pop: {
            ret += "pop";
            break;
        }

        case InstructionType::Const: {
            ret += "const";
            break;
        }

        case InstructionType::Label: {
            ret += "label";
            break;
        }

        case InstructionType::Meta: {
            ret += "meta";
            break;
        }

        default: {
            ret += "<<invalid>>";
            break;
        }
    }

    return ret;
}


std::string to_string(const Ir::Instruction &instr)
{
    using namespace Ir;
    std::string ret = to_string(instr.type);

    switch (instr.type) {
        case InstructionType::Call: {
            ret += " ";
            ret += to_string(instr.arguments[0]);
            ret += " [ ";
            for (size_t i = 2; i < instr.arguments.size(); ++i) {
                ret += to_string(instr.arguments[i]);
                ret += " ";
            }
            ret += "]";
            ret += " ret ";
            ret += to_string(instr.arguments[1]);
            break;
        }

        case InstructionType::Return:
            break;

        case InstructionType::ReturnArg: {
            ret += " ";
            ret += to_string(instr.arguments[0]);
            break;
        }

        case InstructionType::Goto: {
            ret += " ";
            ret += to_string(instr.arguments[0]);
            break;
        }

        case InstructionType::IfEq:
        case InstructionType::IfNeq:
        case InstructionType::IfLt:
        case InstructionType::IfLeq:
        case InstructionType::IfGt:
        case InstructionType::IfGeq: {
            ret += " ";
            ret += to_string(instr.arguments[1]);
            ret += " ";
            ret += to_string(instr.arguments[2]);
            ret += " goto ";
            ret += to_string(instr.arguments[0]);

            break;
        }

        case InstructionType::AddConst: {
            ret += " ";
            ret += to_string(instr.arguments[0]);
            ret += " ";
            ret += to_string(instr.arguments[1]);
            break;
        }

        case InstructionType::Add:
        case InstructionType::Sub:
        case InstructionType::Mul:
        case InstructionType::Div:
        case InstructionType::Mov: {
            ret += " ";
            ret += to_string(instr.arguments[0]);
            ret += " ";
            ret += to_string(instr.arguments[1]);
            break;
        }

        case InstructionType::DivMod: {
            ret += " ";
            ret += to_string(instr.arguments[0]);
            ret += " ";
            ret += to_string(instr.arguments[1]);
            ret += " ";
            ret += to_string(instr.arguments[2]);
            break;
        }

        case InstructionType::Load:
        case InstructionType::Store: {
            ret += " ";
            ret += to_string(instr.arguments[0]);
            ret += " ";
            ret += to_string(instr.arguments[2]);
            ret += "(";
            ret += to_string(instr.arguments[1]);
            ret += ")";
            break;
        }

        case InstructionType::Push:
        case InstructionType::Pop: {
            ret += " ";
            ret += to_string(instr.arguments[0]);
            break;
        }

        case InstructionType::StoreVar:
        case InstructionType::LoadVar:
        case InstructionType::Const: {
            ret += " ";
            ret += to_string(instr.arguments[0]);
            ret += " ";
            ret += to_string(instr.arguments[1]);
            break;
        }

        case InstructionType::Label: {
            ret = to_string(instr.arguments[0]);
            ret += ":";
            break;
        }

        case InstructionType::Meta: {
            ret += " ";
            ret += to_string(instr.arguments[0]);

            if (instr.arguments[0].content == int(MetaTag::External)) {
                ret += to_string(instr.arguments[1]);
            }
            break;
        }

        default: {
            ret += "<<invalid>>";
            break;
        }
    }

    return ret;
}

std::string to_string(const Ir::Block &block)
{
    std::string ret;

    for (auto &instr : block) {
        if (instr.type != Ir::InstructionType::Label) {
            ret += "    ";
        }
        ret += to_string(instr);
        ret += "\n";
    }

    return ret;
}

std::string to_string(const FunctionDef &functionDef)
{
    std::string ret;

    ret += "function ";
    ret += functionDef.name;
    ret += "/";
    ret += std::to_string(functionDef.arguments.size());
    ret += " symbol ";
    ret += functionDef.symName;
    ret += " {\n";
    ret += to_string(functionDef.body);
    ret += "}\n\n";
    return ret;
}

std::string to_string(const SymbolTable &st)
{
    std::string ret;

    for (auto &p : st.argumentToFunction) {
        ret += "map ";
        ret += to_string(p.first);
        ret += " to ";
        ret += p.second;
        ret += "\n";
    }

    ret += "\n";

    for (auto &p : st.functionsMap) {
        if (not p.second.predefined) ret += to_string(p.second);
    }

    return ret;
}

