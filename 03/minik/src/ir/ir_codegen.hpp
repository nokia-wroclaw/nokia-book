#pragma once

#include "ast/nodes.hpp"
#include "ir.hpp"
#include "symbol_table.hpp"

#include <string>
#include <list>

namespace Ir {

class CodeGen {
    typedef std::pair<std::string, Argument> VariableInfo;
//    std::unordered_map<std::string, Argument> variablesMap;
    std::list<VariableInfo> variablesMap;
    int argumentCounter;
    int labelCounter;
    Block block;
    SymbolTable *symbolTable;
    const FunctionDef *currentFunction;

public:
    void translate(std::vector<Ast::Function *> program, SymbolTable &st);

private:
    inline Argument getNextArg() {
        return {ArgumentType::TemporaryRegister, argumentCounter++};
    }

    inline Argument getNextLabel() {
        return {ArgumentType::LabelNumber, labelCounter++};
    }

    inline void addIr(InstructionType type, Arguments args, int extra = 0) {
        block.push_back(Instruction{type, args, extra});
    }

    Block translateFunction(Ast::Function * func, const FunctionDef &funcDef);
    void translateStatements(Ast::Statements stmts);
    VariableInfo translateVariableDecl(Ast::VariableDecl * varDecl);
    void translateIfStatement(Ast::IfStatement * ifStatement);
    void translateWhileStatement(Ast::WhileStatement * whileStatement);
    Argument translateCondition(Ast::ExpressionBase *expr);
    void translateReturnStatement(Ast::ReturnStatement * returnStatement);
    void translateAssignment(Ast::Assignment * assignment);
    void translateAssignment(Argument arg, Ast::ExpressionBase *expr);
    Argument translateFunctionCall(Ast::FunctionCall * functionCall);

    Argument translateExpression(Ast::ExpressionBase *expr);
    Argument translateExpressionAtom(Ast::ExpressionAtom *expr);
    Argument translateExpressionBinary(Ast::ExpressionBinary *expr);
    void translateExpressionBinary(Ast::ExpressionBinary *expr, Argument label);

    Argument translateInteger(Ast::Integer *integer);
    Argument translateIdentifier(const std::string &name, bool loading = false);

    InstructionType translateOp(int op);

    void loadAllNeededArguments(Ast::ExpressionBase *expr);
    void loadAllNeededArguments(Ast::ExpressionAtom *expr);
    void loadAllNeededArguments(Ast::ExpressionBinary *expr);
    void loadAllNeededArguments(Ast::Identifier *id);
    void loadAllNeededArguments(Ast::FunctionCall *funcCall);
    void loadAllNeededArguments(Ast::Statements stmts);
    void loadAllNeededArguments(Ast::VariableDecl *decl);
    void loadAllNeededArguments(Ast::Assignment *assign);
};

} // namespace Ir

