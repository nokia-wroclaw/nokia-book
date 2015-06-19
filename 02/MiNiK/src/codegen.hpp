#pragma once

#include "store.hpp"
#include "asm.hpp"
#include "nodes.hpp"

#include <unordered_map>
#include <string>
#include <vector>
#include <algorithm>
#include <stack>
#include <memory>


typedef std::unordered_map<std::string, FunctionDef> FunctionsMap;

class CodeGen {
    FunctionsMap functionsMap;
    StoreManager store;
    Asm assembler;

public:
    std::string translate(std::vector<Ast::Function *> program) {
        createFunctionsMap(program);

        assembler.createAndAddBlockWithoutLabel(
                {".text", ".global __MINIK_main"});

        translateFunctions(program);

        assembler.createAndAddBlockWithoutLabel(
                {".ident \"minik compiler by Michal Bartkowiak @ Nokia\""});

        return assembler.print();
    }

private:
    void createFunctionsMap(Ast::Functions functions) {
        for (auto func: functions) {
            auto functionDef =
                FunctionDef{
                    func->name,
                    "__MINIK_" + func->name,
                    func->args.size(),
                    computeFrameSize(func->statements) * WordSize,
                    false};

            functionsMap[func->name] = functionDef;
        }

        functionsMap["read"] = FunctionDef{"read", "__MINIK_read", 0, 0, true};
        functionsMap["write"] = FunctionDef{"write", "__MINIK_write", 1, 0, true};
    }

    void translateFunctions(Ast::Functions functions) {
        for (auto func: functions)
            translateFunction(func);
    }

    void translateFunction(Ast::Function * func) {
        auto funcDef = functionsMap[func->name];
        if (funcDef.predefined)
            return;

        store.newFunction(funcDef);

        assembler.createAndAddBlockWithLabel(
                {"enter $" + std::to_string(funcDef.frameSize) + ", $0"},
                funcDef.symName);

        for (auto argIt = func->args.rbegin(); argIt != func->args.rend(); ++argIt)
            store.addArgumentVariable(*argIt);

        translateStatements(func->statements);

        if (func->statements.empty()
            or func->statements.back()->type() != Ast::NodeType::ReturnStatement)
            assembler.createAndAddBlockWithoutLabel({"leave", "ret"});
    }

    std::size_t computeFrameSize(Ast::Statements stmts) {
        std::size_t localVars = 0;
        std::size_t depth = 0;

        for (auto stmt : stmts) {
            switch (stmt->type()) {
                case Ast::NodeType::VariableDecl:
                    ++localVars;
                    break;
                case Ast::NodeType::IfStatement:
                {
                    auto ifStmt = Ast::cast<Ast::NodeType::IfStatement>(stmt);
                    auto ifDepth = computeFrameSize(ifStmt->statements);
                    depth = std::max(depth, localVars + ifDepth);
                    break;
                }
                case Ast::NodeType::WhileStatement:
                {
                    auto whileStmt = Ast::cast<Ast::NodeType::WhileStatement>(stmt);
                    auto whileDepth = computeFrameSize(whileStmt->statements);
                    depth = std::max(depth, localVars + whileDepth);
                    break;
                }
                default:
                    break;
            }
        }

        return std::max(depth, localVars);
    }

    void translateStatements(Ast::Statements stmts) {
        auto variablesStack = std::stack<std::string>();
        for (auto stmt: stmts)
            switch (stmt->type()) {
                case Ast::NodeType::VariableDecl:
                    variablesStack.push(translateVariableDecl(
                        Ast::cast<Ast::NodeType::VariableDecl>(stmt)));
                    break;
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
        for (auto i = 0u; i < variablesStack.size(); ++i) {
            store.popLocalVariable(variablesStack.top());
            variablesStack.pop();
        }
    }

    std::string translateVariableDecl(Ast::VariableDecl * varDecl) {
        store.pushLocalVariable(varDecl->name);
        translateAssignment(varDecl->name, varDecl->expr);
        return varDecl->name;
    }

    void translateIfStatement(Ast::IfStatement * ifStatement) {
        auto leaveBlock = assembler.createBlockWithLabel();

        translateExpression(ifStatement->expr);
        translateCondition(ifStatement->expr, leaveBlock.getLabel());
        translateStatements(ifStatement->statements);

        assembler.addBlock(leaveBlock);
    }

    void translateWhileStatement(Ast::WhileStatement * whileStatement) {
        auto enterBlock = assembler.createBlockWithLabel();
        auto leaveBlock = assembler.createBlockWithLabel();

        assembler.addBlock(enterBlock);

        translateExpression(whileStatement->expr);
        translateCondition(whileStatement->expr, leaveBlock.getLabel());
        translateStatements(whileStatement->statements);

        assembler.createAndAddBlockWithoutLabel({"jmp " + enterBlock.getLabel()});
        assembler.addBlock(leaveBlock);
    }

    void translateCondition(Ast::ExpressionBase * expr, std::string label) {
        auto condBlock = assembler.createBlockWithoutLabel();
        if (expr->type() == Ast::NodeType::ExpressionAtom) {
            condBlock.addInstr("cmpl $0, %eax");
            condBlock.addInstr("jz " + label);
        } else {
            assert(expr->type() == Ast::NodeType::ExpressionBinary);
            auto binExpr = Ast::cast<Ast::NodeType::ExpressionBinary>(expr);
            switch (binExpr->op) {
                case TMUL:
                case TDIV:
                case TPLUS:
                case TMINUS:
                    condBlock.addInstr("cmpl $0, %eax");
                    condBlock.addInstr("jz " + label);
                    break;
                case TCLT: condBlock.addInstr("jle " + label); break;
                case TCLE: condBlock.addInstr("jl " + label); break;
                case TCGT: condBlock.addInstr("jge " + label); break;
                case TCGE: condBlock.addInstr("jg " + label); break;
                case TCEQ: condBlock.addInstr("jne " + label); break;
                case TCNE: condBlock.addInstr("je " + label); break;
                default: assert(false); break;
            }

        }
        assembler.addBlock(condBlock);
    }

    void translateReturnStatement(Ast::ReturnStatement * returnStatement) {
        translateExpression(returnStatement->expr);
        assembler.createAndAddBlockWithoutLabel({"leave", "ret"});
    }

    void translateAssignment(Ast::Assignment * assignment) {
        translateAssignment(assignment->name, assignment->expr);
    }

    void translateAssignment(std::string id, Ast::ExpressionBase * expr) {
        translateExpression(expr);
        sendEaxToLocalVariable(store.getVariableOffset(id));
    }

    void translateFunctionCall(Ast::FunctionCall * functionCall) {
        auto name = functionCall->name;
        auto funcDef = functionsMap[name];

        auto args = functionCall->args;
        for (auto argIt = args.rbegin(); argIt != args.rend(); ++argIt) {
            translateIdentifier(*argIt);
            assembler.createAndAddBlockWithoutLabel({"pushl %eax"});
        }
        auto funcCallBlock = assembler.createBlockWithoutLabel();
        funcCallBlock.addInstr("call " + funcDef.symName);
        if (0 != funcDef.numberOfArgs)
            funcCallBlock.addInstr("addl $" + std::to_string(funcDef.numberOfArgs * WordSize) + ", %esp");
        assembler.addBlock(funcCallBlock);
    }

    void translateExpression(Ast::ExpressionBase *expr) {
        switch (expr->type()) {
            case Ast::NodeType::ExpressionAtom:
                translateExpressionAtom(
                    Ast::cast<Ast::NodeType::ExpressionAtom>(expr));
                break;
            case Ast::NodeType::ExpressionBinary:
                translateExpressionBinary(
                    Ast::cast<Ast::NodeType::ExpressionBinary>(expr));
                break;
            default:
                assert(false);
                break;
        }
    }

    void translateExpressionAtom(Ast::ExpressionAtom *expr) {
        switch (expr->node->type()) {
            case Ast::NodeType::Integer:
                translateInteger(
                    Ast::cast<Ast::NodeType::Integer>(expr->node));
                break;
            case Ast::NodeType::Identifier:
                translateIdentifier(
                    Ast::cast<Ast::NodeType::Identifier>(expr->node)->val);
                break;
            case Ast::NodeType::ExpressionAtom:
            case Ast::NodeType::ExpressionBinary:
                translateExpression(
                    Ast::cast<Ast::NodeType::ExpressionBase>(expr->node));
                break;
            case Ast::NodeType::FunctionCall:
                translateFunctionCall(
                    Ast::cast<Ast::NodeType::FunctionCall>(expr->node));
                break;
            default:
                assert(false);
                break;

        }
    }

    void translateExpressionBinary(Ast::ExpressionBinary *expr) {
        translateExpression(expr->lhs);
        assembler.createAndAddBlockWithoutLabel({"pushl %eax"});
        translateExpression(expr->rhs);
        auto op = translateOp(expr->op);
        if (TMINUS == expr->op)
            assembler.createAndAddBlockWithoutLabel(
                {"popl %ebx", op + " %eax, %ebx", "movl %ebx, %eax"});
        else if (TDIV == expr->op)
            assembler.createAndAddBlockWithoutLabel(
                {"movl %eax, %ebx", "popl %eax", "cltd", op + " %ebx"});
        else
            assembler.createAndAddBlockWithoutLabel({op + " (%esp), %eax", "popl %ebx"});
    }

    void fetchVariableToRegister(std::string name, std::string reg) {
        auto offset = store.getVariableOffset(name);
        assembler.createAndAddBlockWithoutLabel(
                {"movl " + std::to_string(offset) + "(%ebp), %" + reg});
    }

    void sendEaxToLocalVariable(int offset) {
        assembler.createAndAddBlockWithoutLabel(
                {"movl %eax, " + std::to_string(offset) + "(%ebp)"});
    }

    void translateIdentifier(std::string name) {
        fetchVariableToRegister(name, "eax");
    }

    void translateInteger(Ast::Integer *integer) {
        assembler.createAndAddBlockWithoutLabel(
                {"movl $" + std::to_string(integer->val) + ", %eax"});
    }

    std::string translateOp(int op) {
        switch(op) {
            case TMUL: return "imul";
            case TDIV: return "idivl";
            case TPLUS: return "addl";
            case TMINUS: return "subl";
            case TCEQ:
            case TCNE:
            case TCLT:
            case TCLE:
            case TCGT:
            case TCGE:
                return "cmpl";
            default: assert(false); return "";
        }
    }
};

