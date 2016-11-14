#include "ir_codegen.hpp"
//#include "x86_assembler.hpp"
#include <algorithm>
#include <iostream>

namespace Ir {

void CodeGen::translate(
        std::vector<Ast::Function *> program,
        SymbolTable &st) {
    symbolTable = &st;
    labelCounter = 0;
    for (auto func: program) {
        auto functionBody = translateFunction(func, st.getFunction(func->name));
        st.addBodyToFunction(func->name, functionBody);
    }
}

Block CodeGen::translateFunction(Ast::Function * func, const FunctionDef &funcDef) {
    if (funcDef.predefined)
        return Block{};

    argumentCounter = 0;
    variablesMap.clear();
    block.clear();
    currentFunction = &funcDef;

    translateStatements(func->statements);

    currentFunction = nullptr;

    return block;
}

void CodeGen::translateStatements(Ast::Statements stmts) {
    auto localVariablesCounter = variablesMap.size();

    for (auto stmt: stmts) {
        switch (stmt->type()) {
            case Ast::NodeType::VariableDecl:
            {
                auto variable = translateVariableDecl(
                    Ast::cast<Ast::NodeType::VariableDecl>(stmt));
                variablesMap.push_front(variable);
                ++localVariablesCounter;
                break;
            }
            case Ast::NodeType::IfStatement:
                translateIfStatement(
                    Ast::cast<Ast::NodeType::IfStatement>(stmt));
                break;
            case Ast::NodeType::WhileStatement:
                translateWhileStatement(
                    Ast::cast<Ast::NodeType::WhileStatement>(stmt));
                break;
            case Ast::NodeType::ReturnStatement:
                translateReturnStatement(
                    Ast::cast<Ast::NodeType::ReturnStatement>(stmt));
                break;
            case Ast::NodeType::Assignment:
                translateAssignment(
                    Ast::cast<Ast::NodeType::Assignment>(stmt));
                break;
            case Ast::NodeType::FunctionCall:
                translateFunctionCall(
                    Ast::cast<Ast::NodeType::FunctionCall>(stmt));
                break;
            default:
                assert(false);
                break;
        }
    }

    localVariablesCounter = variablesMap.size() - localVariablesCounter;
    auto eraseEnd = variablesMap.begin();
    std::advance(eraseEnd, localVariablesCounter);
    variablesMap.erase(variablesMap.begin(), eraseEnd);
}

CodeGen::VariableInfo CodeGen::translateVariableDecl(Ast::VariableDecl * varDecl) {
    auto argNumber = getNextArg();
    translateAssignment(argNumber, varDecl->expr);
    return VariableInfo{varDecl->name, argNumber};
}

void CodeGen::translateIfStatement(Ast::IfStatement * ifStatement) {
    auto label = translateCondition(ifStatement->expr);
    translateStatements(ifStatement->statements);
    addIr(InstructionType::Label, Arguments{label});
}

void CodeGen::translateWhileStatement(Ast::WhileStatement * whileStatement) {
    auto enterWhile = getNextLabel();

    loadAllNeededArguments(whileStatement->expr);
    loadAllNeededArguments(whileStatement->statements);

    addIr(InstructionType::Label, Arguments{enterWhile});

    auto leaveWhile = translateCondition(whileStatement->expr);
    translateStatements(whileStatement->statements);

    addIr(InstructionType::Goto, Arguments{enterWhile});
    addIr(InstructionType::Label, Arguments{leaveWhile});
}

Argument CodeGen::translateCondition(Ast::ExpressionBase *expr) {
    auto label = getNextLabel();
    switch (expr->type()) {
        case Ast::NodeType::ExpressionAtom:
        {
            auto lhsArg =
                translateExpressionAtom(
                    Ast::cast<Ast::NodeType::ExpressionAtom>(expr));
            auto rhsArg = getNextArg();
            addIr(InstructionType::Const, Arguments{rhsArg, {ArgumentType::Integer, 0}});
            addIr(InstructionType::IfEq, Arguments{label, lhsArg, rhsArg});
            break;
        }
        case Ast::NodeType::ExpressionBinary:
            translateExpressionBinary(
                Ast::cast<Ast::NodeType::ExpressionBinary>(expr), label);
            break;
        default:
            throw std::runtime_error("kamieni kupa");
    }

    return label;
}

void CodeGen::translateReturnStatement(Ast::ReturnStatement * returnStatement) {
    auto result = translateExpression(returnStatement->expr);
    addIr(InstructionType::ReturnArg, Arguments{result});
}

void CodeGen::translateAssignment(Ast::Assignment * assignment) {
    auto arg = translateIdentifier(assignment->name);
    translateAssignment(arg, assignment->expr);
}

void CodeGen::translateAssignment(Argument arg, Ast::ExpressionBase *expr) {
    auto exprResult = translateExpression(expr);
    addIr(InstructionType::Mov, Arguments{arg, exprResult});
}

Argument CodeGen::translateFunctionCall(Ast::FunctionCall * functionCall) {
    auto name = functionCall->name;
    const auto &funcDef = symbolTable->getFunction(name);
    auto result = getNextArg();

    auto args = functionCall->args;
    Arguments argsForCall = {funcDef.id, result};
    for (const auto &arg: args) {
        auto reg = translateIdentifier(arg);
        argsForCall.push_back(reg);
    }

    addIr(InstructionType::Meta, Arguments{Argument::Meta(MetaTag::CallerSave)});
    addIr(InstructionType::Call, argsForCall);
    addIr(InstructionType::Meta, Arguments{Argument::Meta(MetaTag::CallerRestore)});

    return result;
}

Argument CodeGen::translateExpression(Ast::ExpressionBase *expr) {
    switch (expr->type()) {
        case Ast::NodeType::ExpressionAtom:
            return
                translateExpressionAtom(
                    Ast::cast<Ast::NodeType::ExpressionAtom>(expr));
        case Ast::NodeType::ExpressionBinary:
            return
                translateExpressionBinary(
                    Ast::cast<Ast::NodeType::ExpressionBinary>(expr));
        default:
            throw std::runtime_error("kamieni kupa");
    }
}

Argument CodeGen::translateExpressionAtom(Ast::ExpressionAtom *expr) {
    switch (expr->node->type()) {
        case Ast::NodeType::Integer:
            return
                translateInteger(
                    Ast::cast<Ast::NodeType::Integer>(expr->node));
        case Ast::NodeType::Identifier:
            return
                translateIdentifier(
                    Ast::cast<Ast::NodeType::Identifier>(expr->node)->val);
        case Ast::NodeType::ExpressionAtom:
        case Ast::NodeType::ExpressionBinary:
            return
                translateExpression(
                    Ast::cast<Ast::NodeType::ExpressionBase>(expr->node));
        case Ast::NodeType::FunctionCall:
            return
                translateFunctionCall(
                    Ast::cast<Ast::NodeType::FunctionCall>(expr->node));
        default:
            throw std::runtime_error("kamieni kupa");
    }
}

Argument CodeGen::translateExpressionBinary(Ast::ExpressionBinary *expr) {
    auto lhsResult = translateExpression(expr->lhs);
    auto rhsResult = translateExpression(expr->rhs);
    auto exprResult = getNextArg();
    addIr(InstructionType::Mov, Arguments{exprResult, lhsResult});
    addIr(translateOp(expr->op), Arguments{exprResult, rhsResult});
    return exprResult;
}

void CodeGen::translateExpressionBinary(Ast::ExpressionBinary *expr, Argument label) {
    auto lhsResult = translateExpression(expr->lhs);
    auto rhsResult = translateExpression(expr->rhs);
    addIr(translateOp(expr->op), Arguments{label, lhsResult, rhsResult});
}

Argument CodeGen::translateInteger(Ast::Integer *integer) {
    auto result = getNextArg();
    addIr(InstructionType::Const, Arguments{result, {ArgumentType::Integer, integer->val}});
    return result;
}

Argument CodeGen::translateIdentifier(const std::string &name, bool loading) {
    auto resultIt =
        std::find_if(
                variablesMap.cbegin(),
                variablesMap.cend(),
                [&name] (const VariableInfo &vi) { return name == vi.first; });

    // if not present in variables map check the function arguments
    if (variablesMap.cend() == resultIt) {
        const auto &args = currentFunction->arguments;
        auto foundArg = std::find(args.cbegin(), args.cend(), name);
        if (args.cend() == foundArg) {
            if (not loading)
                throw std::runtime_error("kamieni kupa");
            return Argument::Integer(0);
        }
        const auto distance = Argument::Integer(-1 - (int)std::distance(args.cbegin(), foundArg));
        auto result = getNextArg();
        addIr(InstructionType::LoadVar, Arguments{result, distance});
        variablesMap.push_front(VariableInfo{name, result});
        return result;
    } else {
        return resultIt->second;
    }
}

InstructionType CodeGen::translateOp(int op) {
    switch(op) {
        case TMUL:   return InstructionType::Mul;
        case TDIV:   return InstructionType::Div;
        case TPLUS:  return InstructionType::Add;
        case TMINUS: return InstructionType::Sub;
        case TCEQ:   return InstructionType::IfNeq;
        case TCNE:   return InstructionType::IfEq;
        case TCLT:   return InstructionType::IfGeq;
        case TCLE:   return InstructionType::IfGt;
        case TCGT:   return InstructionType::IfLeq;
        case TCGE:   return InstructionType::IfLt;
        default:
            assert(false);
            return InstructionType::Meta;
    }
}

void CodeGen::loadAllNeededArguments(Ast::ExpressionBase *expr) {
    switch (expr->type()) {
        case Ast::NodeType::ExpressionAtom:
            loadAllNeededArguments(Ast::cast<Ast::NodeType::ExpressionAtom>(expr));
            break;
        case Ast::NodeType::ExpressionBinary:
            loadAllNeededArguments(Ast::cast<Ast::NodeType::ExpressionBinary>(expr));
            break;
        default:
            assert(false);
    }
}

void CodeGen::loadAllNeededArguments(Ast::ExpressionAtom *expr) {
    switch (expr->node->type()) {
        case Ast::NodeType::Identifier:
            loadAllNeededArguments(Ast::cast<Ast::NodeType::Identifier>(expr->node));
            break;
        case Ast::NodeType::ExpressionAtom:
        case Ast::NodeType::ExpressionBinary:
            loadAllNeededArguments(Ast::cast<Ast::NodeType::ExpressionBase>(expr->node));
            break;
        case Ast::NodeType::FunctionCall:
            loadAllNeededArguments(Ast::cast<Ast::NodeType::FunctionCall>(expr->node));
            break;
        case Ast::NodeType::Integer:
            break;
        default:
            assert(false);
    }
}

void CodeGen::loadAllNeededArguments(Ast::ExpressionBinary *expr) {
    loadAllNeededArguments(expr->lhs);
    loadAllNeededArguments(expr->rhs);
}

void CodeGen::loadAllNeededArguments(Ast::Identifier *id) {
    translateIdentifier(id->val, true);
}

void CodeGen::loadAllNeededArguments(Ast::FunctionCall *functionCall) {
    auto args = functionCall->args;
    for (const auto &arg: args)
        auto reg = translateIdentifier(arg, true);
}

void CodeGen::loadAllNeededArguments(Ast::Statements stmts) {
    for (auto stmt: stmts) {
        switch (stmt->type()) {
            case Ast::NodeType::VariableDecl:
                loadAllNeededArguments(Ast::cast<Ast::NodeType::VariableDecl>(stmt));
                break;
            case Ast::NodeType::IfStatement: // 'if' will handle the loading by itself
            case Ast::NodeType::WhileStatement: // 'while' will handle the loading by itself
            case Ast::NodeType::ReturnStatement: // will only fire once
                break;
            case Ast::NodeType::Assignment:
                loadAllNeededArguments(Ast::cast<Ast::NodeType::Assignment>(stmt));
                break;
            case Ast::NodeType::FunctionCall:
                loadAllNeededArguments(Ast::cast<Ast::NodeType::FunctionCall>(stmt));
                break;
            default:
                assert(false);
        }
    }
}

void CodeGen::loadAllNeededArguments(Ast::VariableDecl *varDecl) {
    loadAllNeededArguments(varDecl->expr);
}

void CodeGen::loadAllNeededArguments(Ast::Assignment *assign) {
    translateIdentifier(assign->name, true);
    loadAllNeededArguments(assign->expr);
}

} // namespace Ir

