#pragma once

#include <vector>
#include <string>
#include <cassert>


namespace {
const std::string defaultIndent = "    ";
std::string opToString(int op);
} // namespace


namespace Ast {

enum class NodeType {
    Undefined,
    Identifier,
    Integer,
    Function,
    VariableDecl,
    FunctionCall,
    ExpressionBase,
    ExpressionAtom,
    ExpressionBinary,
    Assignment,
    IfStatement,
    WhileStatement,
    ReturnStatement
};


struct Node {
    virtual ~Node() {}
    virtual NodeType type() const { return NodeType::Undefined; }
    virtual std::string toString(std::string indent = "") = 0;
};

struct ExpressionBase : public Node {
};

struct Statement : public Node {
};


struct Function;
struct Statement;
struct Identifier;

typedef std::vector<Function *> Functions;
typedef std::vector<Statement *> Statements;
typedef std::vector<std::string> Arguments;

struct Identifier : public Node {
    std::string val;

    Identifier(std::string val) : val(val) { }
    virtual NodeType type() const { return NodeType::Identifier; }
    virtual std::string toString(std::string indent = "") override {
        return indent + "<Identifier> '" + val + "'" + "\n";
    }
};

struct Integer : public Node {
    int val;

    Integer(int val) : val(val) { }
    virtual NodeType type() const { return NodeType::Integer; }
    virtual std::string toString(std::string indent = "") override {
        return indent + "<Integer> '" + std::to_string(val) + "'" + "\n";
    }
};

struct Function : public Node {
    std::string name;
    Arguments args;
    Statements statements;

    Function(std::string name, Arguments args, Statements statements) :
        name(name), args(args), statements(statements) { }
    ~Function() {
        for (auto stmt: statements)
            delete stmt;
    }
    virtual NodeType type() const { return NodeType::Function; }
    virtual std::string toString(std::string indent = "") override {
        auto output = indent + "<Function> '" + name + "'\n";
        if (not args.empty()) {
            output += indent + defaultIndent + "<Arguments>";
            for (auto arg: args)
                output += " '" + arg + "'";
            output += "\n";
        }
        for (auto stmt: statements)
            output += stmt->toString(indent + defaultIndent);
        return output;
    }
};

struct VariableDecl : public Statement {
    std::string name;
    ExpressionBase *expr;

    VariableDecl(std::string name, ExpressionBase *expr) :
        name(name), expr(expr) { }
    ~VariableDecl() {
        delete expr;
    }
    virtual NodeType type() const { return NodeType::VariableDecl; }
    virtual std::string toString(std::string indent = "") override {
        auto output = indent + "<VariableDecl> '" + name + "'\n";
        output += expr->toString(indent + defaultIndent);
        return output;
    }
};

struct FunctionCall : public Statement {
    std::string name;
    Arguments args;

    FunctionCall(std::string name, Arguments args) : name(name), args(args) { }
    virtual NodeType type() const { return NodeType::FunctionCall; }
    virtual std::string toString(std::string indent = "") override {
        auto output = indent + "<FunctionCall> '" + name + "'\n";
        if (not args.empty()) {
            output += indent + defaultIndent + "<Arguments>";
            for (auto arg: args)
                output += " '" + arg + "'";
            output += "\n";
        }
        return output;
    }
};

struct ExpressionAtom : public ExpressionBase {
    Node *node;
    bool minus;

    ExpressionAtom(Node *node, bool minus = false) : node(node), minus(minus) { }
    ~ExpressionAtom() {
        delete node;
    }
    virtual NodeType type() const { return NodeType::ExpressionAtom; }
    virtual std::string toString(std::string indent = "") override {
        auto output = indent + "<ExpressionAtom>\n";
        if (minus)
            output += indent + defaultIndent + "<Operation> '-'\n";
        output += node->toString(indent + defaultIndent);
        return output;
    }
};

struct ExpressionBinary : public ExpressionBase {
    int op;
    ExpressionBase *lhs;
    ExpressionBase *rhs;

    ExpressionBinary(ExpressionBase *lhs, int op, ExpressionBase *rhs) :
        op(op), lhs(lhs), rhs(rhs) { }
    ~ExpressionBinary() {
        delete lhs;
        delete rhs;
    }
    virtual NodeType type() const { return NodeType::ExpressionBinary; }
    virtual std::string toString(std::string indent = "") override {
        auto output = indent + "<ExpressionBinary>\n";
        output += lhs->toString(indent + defaultIndent);
        if (rhs) {
            output += indent + defaultIndent + opToString(op);
            output += rhs->toString(indent + defaultIndent);
        }
        return output;
    }
};

struct Assignment : public Statement {
    std::string name;
    ExpressionBase *expr;

    Assignment(std::string name, ExpressionBase *expr) : name(name), expr(expr) { }
    ~Assignment() {
        delete expr;
    }
    virtual NodeType type() const { return NodeType::Assignment; }
    virtual std::string toString(std::string indent = "") override {
        auto output = indent + "<Assignment> '" + name + "'\n";
        output += expr->toString(indent + defaultIndent);
        return output;
    }
};

struct IfStatement : public Statement {
    ExpressionBase *expr;
    Statements statements;

    IfStatement(ExpressionBase *expr, Statements statements)
        : expr(expr), statements(statements) { }
    ~IfStatement() {
        delete expr;
        for (auto stmt: statements)
            delete stmt;
    }
    virtual NodeType type() const { return NodeType::IfStatement; }
    virtual std::string toString(std::string indent = "") override {
        auto output = indent + "<IfStatement>\n";
        output += expr->toString(indent + defaultIndent);
        for (auto stmt: statements)
            output += stmt->toString(indent + defaultIndent);
        return output;
    }
};

struct WhileStatement : public Statement {
    ExpressionBase *expr;
    Statements statements;

    WhileStatement(ExpressionBase *expr, Statements statements)
        : expr(expr), statements(statements) { }
    ~WhileStatement() {
        delete expr;
        for (auto stmt: statements)
            delete stmt;
    }
    virtual NodeType type() const { return NodeType::WhileStatement; }
    virtual std::string toString(std::string indent = "") override {
        auto output = indent + "<WhileStatement>\n";
        output += expr->toString(indent + defaultIndent);
        for (auto stmt: statements)
            output += stmt->toString(indent + defaultIndent);
        return output;
    }
};

struct ReturnStatement : public Statement {
    ExpressionBase *expr;

    ReturnStatement(ExpressionBase *expr) : expr(expr) { }
    ~ReturnStatement() {
        delete expr;
    }
    virtual NodeType type() const { return NodeType::ReturnStatement; }
    virtual std::string toString(std::string indent = "") override {
        auto output = indent + "<ReturnStatement>\n";
        output += expr->toString(indent + defaultIndent);
        return output;
    }
};


template<typename UpCastedType>
UpCastedType * castTo(Node * node) {
    auto casted = dynamic_cast<UpCastedType *>(node);
    assert(casted != nullptr);
    return casted;
}

template<enum NodeType> struct NodeTypeTraits;
template<> struct NodeTypeTraits<NodeType::Identifier> { typedef Identifier type; };
template<> struct NodeTypeTraits<NodeType::Integer> { typedef Integer type; };
template<> struct NodeTypeTraits<NodeType::Function> { typedef Function type; };
template<> struct NodeTypeTraits<NodeType::VariableDecl> { typedef VariableDecl type; };
template<> struct NodeTypeTraits<NodeType::FunctionCall> { typedef FunctionCall type; };
template<> struct NodeTypeTraits<NodeType::ExpressionBase> { typedef ExpressionBase type; };
template<> struct NodeTypeTraits<NodeType::ExpressionAtom> { typedef ExpressionAtom type; };
template<> struct NodeTypeTraits<NodeType::ExpressionBinary> { typedef ExpressionBinary type; };
template<> struct NodeTypeTraits<NodeType::Assignment> { typedef Assignment type; };
template<> struct NodeTypeTraits<NodeType::IfStatement> { typedef IfStatement type; };
template<> struct NodeTypeTraits<NodeType::WhileStatement> { typedef WhileStatement type; };
template<> struct NodeTypeTraits<NodeType::ReturnStatement> { typedef ReturnStatement type; };

template<NodeType NodeTypeValue>
typename NodeTypeTraits<NodeTypeValue>::type * cast(Node * node) {
    return castTo<typename NodeTypeTraits<NodeTypeValue>::type>(node);
}

} // namespace Ast

#include "minik_parser.hpp"
namespace {
std::string opToString(int op) {
    auto output = std::string("<Operation> '");
    switch(op) {
        case TCEQ: output += "=="; break;
        case TCNE: output += "!="; break;
        case TCLT: output += "<"; break;
        case TCLE: output += "<="; break;
        case TCGT: output += ">"; break;
        case TCGE: output += ">="; break;
        case TPLUS: output += "+"; break;
        case TMINUS: output += "-"; break;
        case TMUL: output += "*"; break;
        case TDIV: output += "/"; break;
        default: break;
    }
    return output + "'\n";
}
} // namespace
