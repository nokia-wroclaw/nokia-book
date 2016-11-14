#pragma once

#include <list>
#include <vector>
#include <set>
#include <functional>

namespace Ir {

enum class InstructionType {
    Call,   // <ID> <REGISTER> <REGISTER...>..
    ReturnArg, // <REGISTER>
    Return,// 
    Goto,   // <LABEL>
    IfEq,   // <LABEL> <REGISTER> <REGISTER>
    IfNeq,
    IfLt,
    IfLeq,
    IfGt,
    IfGeq,
    Add,        // <REGISTER> <REGISTER> 
    AddConst,   // <REGISTER> <INT> 
    Sub,        // <REGISTER> <REGISTER>
    Mul,        // <REGISTER> <REGISTER>
    Div,        // <REGISTER> <REGISTER>
    DivMod,     // <REGISTER> <REGISTER> <REGISTER>
    Mov,        // <REGISTER> <REGISTER>
    Load,       // <REGISTER> <REGISTER> <INT>
    Store,      // <REGISTER> <REGISTER> <INT>
    LoadVar,    // <REGISTER> <INT>
    StoreVar,   // <REGISTER> <INT>
    Push,       // <REGISTER>
    Pop,        // <REGISTER>
    Const,      // <REGISTER> <INT>
    Label,      // <LABEL>
    Meta        // <META TAG> ...
};


enum class MetaTag {
    Exit,
    Entry,
    External,
    CallerSave,
    CallerRestore
};

enum class ArgumentType
{
    TemporaryRegister,
    AnyHardRegister,
    PinnedHardRegister,
    PinnedSpecialHardRegister,
    FunctionNumber,
    LabelNumber,
    Integer,
    Meta
};

struct Argument
{
    static constexpr Argument Integer(int i)
    {
        return {ArgumentType::Integer, i};
    }

    static constexpr Argument Meta(MetaTag tag)
    {
        return {ArgumentType::Meta, int(tag)};
    }

    static constexpr Argument AnyHardRegister(int i)
    {
        return {ArgumentType::AnyHardRegister, i};
    }

    static constexpr Argument PinnedHardRegister(int i)
    {
        return {ArgumentType::PinnedHardRegister, i};
    }

    static constexpr Argument PinnedSpecialHardRegister(int i)
    {
        return {ArgumentType::PinnedSpecialHardRegister, i};
    }

    ArgumentType    type;
    int             content;

    inline bool is(ArgumentType expectedType) const { return type == expectedType; }

    inline bool isGeneralPurposeHardRegister() const { return is(ArgumentType::AnyHardRegister) or is(ArgumentType::PinnedHardRegister); }

    bool operator==(const Argument &o) const
    {
        return type == o.type and content == o.content;
    }

    bool operator!=(const Argument &o) const
    {
        return not (*this == o);
    }

    bool operator<(const Argument &o) const
    {
        return std::make_tuple((int)type, content) < std::make_tuple((int)o.type, o.content);
    }
};

}

namespace std
{
    template <> struct hash<Ir::Argument>
    {
        typedef Ir::Argument argument_type;
        typedef std::size_t result_type;

        std::size_t operator()(const Ir::Argument& k) const
        {
            return std::hash<int>()((int)k.type) ^ std::hash<int>()(k.content);
        }
    };
}

namespace Ir {
/*
 * <  0 registers
 * >= 0 symbolic registers
 */

inline bool isHardRegister(int i)
{
    return i < 0;
}

inline bool isSymbolicRegister(int i)
{
    return i >= 0;
}


typedef std::vector<Argument> Arguments;

struct Instruction  {
    InstructionType type;
    Arguments arguments;
    int extra;
};

typedef std::list<Instruction> Block;
typedef std::vector<Block> Blocks;

std::set<Argument> computeUsedRegisters(const Block &block);
std::set<Argument> computeUsedRegisters(const Instruction &instr);
std::set<Argument> computeModifiedRegisters(const Block &block);
std::set<Argument> computeAccessedRegisters(const Block &instr);
std::set<Argument> computeModifiedRegisters(const Instruction &instr);
std::set<Argument> computeAccessedRegisters(const Instruction &instr);

Block substituteRegister(const Block x, Argument from, Argument to);

Block cleanup(const Block x);

} // namespace Ir
