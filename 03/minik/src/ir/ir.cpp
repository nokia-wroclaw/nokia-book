#include "ir.hpp"

namespace Ir {

std::set<Argument> computeUsedRegisters(const Instruction &instr)
{
    std::set<Argument> xs;
    for (auto r : computeModifiedRegisters(instr)) {
        xs.insert(r);
    }

    for (auto r : computeAccessedRegisters(instr)) {
        xs.insert(r);
    }
    return xs;
}

std::set<Argument> computeUsedRegisters(const Block &block)
{
    std::set<Argument> xs;
    for (auto &instr : block) {
        for (auto r : computeModifiedRegisters(instr)) {
            xs.insert(r);
        }

        for (auto r : computeAccessedRegisters(instr)) {
            xs.insert(r);
        }
    }
    return xs;
}

std::set<Argument> computeModifiedRegisters(const Block &block)
{
    std::set<Argument> xs;
    for (auto &instr : block) {
        for (auto r : computeModifiedRegisters(instr)) {
            xs.insert(r);
        }
    }
    return xs;
}

std::set<Argument> computeAccessedRegisters(const Block &block)
{
    std::set<Argument> xs;
    for (auto &instr : block) {
        for (auto r : computeAccessedRegisters(instr)) {
            xs.insert(r);
        }
    }
    return xs;
}

std::set<Argument> computeModifiedRegisters(const Instruction &instr)
{
    std::set<Argument> ret;

    switch (instr.type) {
        case InstructionType::Call:
            ret.insert(instr.arguments[1]);
            break;

        case InstructionType::Return:
        case InstructionType::Goto:
        case InstructionType::IfEq:
        case InstructionType::IfNeq:
        case InstructionType::IfLt:
        case InstructionType::IfLeq:
        case InstructionType::IfGt:
        case InstructionType::IfGeq:
            break;

        case InstructionType::Add:
        case InstructionType::AddConst:
        case InstructionType::Sub:
        case InstructionType::Mul:
        case InstructionType::Div:
        case InstructionType::Mov:
            ret.insert(instr.arguments[0]);
            break;

        case InstructionType::DivMod:
            ret.insert(instr.arguments[0]);
            ret.insert(instr.arguments[1]);
            break;

        case InstructionType::Load:
            ret.insert(instr.arguments[0]);
            break;

        case InstructionType::Store:
            break;

        case InstructionType::LoadVar:
            ret.insert(instr.arguments[0]);
            break;

        case InstructionType::Push:
            break;

        case InstructionType::Pop:
            ret.insert(instr.arguments[0]);
            break;

        case InstructionType::Const:
            ret.insert(instr.arguments[0]);
            break;

        case InstructionType::Label:
        case InstructionType::Meta:
            break;

        default:
            break;
    }

    return ret;
}

std::set<Argument> computeAccessedRegisters(const Instruction &instr)
{
    std::set<Argument> ret;

    switch (instr.type) {
        case InstructionType::Call:
            for (std::size_t i = 2; i < instr.arguments.size(); i++) {
                ret.insert(instr.arguments[i]);
            }
            break;

        case InstructionType::ReturnArg:
            ret.insert(instr.arguments[0]);
            break;

        case InstructionType::Return:
        case InstructionType::Goto:
            break;

        case InstructionType::IfEq:
        case InstructionType::IfNeq:
        case InstructionType::IfLt:
        case InstructionType::IfLeq:
        case InstructionType::IfGt:
        case InstructionType::IfGeq:
            ret.insert(instr.arguments[1]);
            ret.insert(instr.arguments[2]);
            break;

        case InstructionType::AddConst:
            ret.insert(instr.arguments[0]);
            break;

        case InstructionType::Add:
        case InstructionType::Sub:
        case InstructionType::Mul:
        case InstructionType::Div:
            ret.insert(instr.arguments[0]);
            ret.insert(instr.arguments[1]);
            break;

        case InstructionType::DivMod:
            ret.insert(instr.arguments[0]);
            ret.insert(instr.arguments[1]);
            ret.insert(instr.arguments[2]);
            break;

        case InstructionType::Mov:
        case InstructionType::Load:
            ret.insert(instr.arguments[1]);
            break;

        case InstructionType::Store:
            ret.insert(instr.arguments[0]);
            ret.insert(instr.arguments[1]);
            break;

        case InstructionType::LoadVar:
            break;

        case InstructionType::Push:
        case InstructionType::StoreVar:
            ret.insert(instr.arguments[0]);
            break;

        case InstructionType::Pop:
        case InstructionType::Const:
        case InstructionType::Label:
        case InstructionType::Meta:
            break;

        default:
            break;
    }

    return ret;
}

Block substituteRegister(const Block x, Argument from, Argument to)
{
    Block result;

    auto alignReg = [&] (Argument &x)
    {
        if (x == from) x = to;
    };

    for (auto instr : x) {
        switch (instr.type) {
            case InstructionType::Call:
                for (std::size_t i = 1; i < instr.arguments.size(); i++) {
                    alignReg(instr.arguments[i]);
                }
                break;

            case InstructionType::ReturnArg:
                alignReg(instr.arguments[0]);
                break;

            case InstructionType::Return:
            case InstructionType::Goto:
            case InstructionType::Label:
            case InstructionType::Meta:
                break;

            case InstructionType::IfEq:
            case InstructionType::IfNeq:
            case InstructionType::IfLt:
            case InstructionType::IfLeq:
            case InstructionType::IfGt:
            case InstructionType::IfGeq:
                alignReg(instr.arguments[1]);
                alignReg(instr.arguments[2]);
                break;

            case InstructionType::AddConst:
            case InstructionType::LoadVar:
            case InstructionType::StoreVar:
            case InstructionType::Push:
            case InstructionType::Pop:
            case InstructionType::Const:
                alignReg(instr.arguments[0]);
                break;

            case InstructionType::Add:
            case InstructionType::Sub:
            case InstructionType::Mul:
            case InstructionType::Div:
            case InstructionType::Mov:
            case InstructionType::Store:
            case InstructionType::Load:
                alignReg(instr.arguments[0]);
                alignReg(instr.arguments[1]);
                break;

            case InstructionType::DivMod:
                alignReg(instr.arguments[0]);
                alignReg(instr.arguments[1]);
                alignReg(instr.arguments[2]);
                break;
        }
        result.push_back(instr);
    }

    return result;
}

Block cleanup(const Block x)
{
    Block result;

    for (auto instr : x) {
        if (instr.type == InstructionType::Mov) {
            if (instr.arguments[0] != instr.arguments[1]) {
                result.push_back(instr);
            }
        } else {
            result.push_back(instr);
        }
    }

    return result;
}

}
