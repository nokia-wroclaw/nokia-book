#pragma once

#include "messages.hpp"
#include "nodes.hpp"

#include <unordered_map>
#include <string>
#include <vector>
#include <algorithm>


namespace Checks {

struct FunctionDef {
    std::string name;
    std::size_t numberOfArgs;
};

typedef std::unordered_map<std::string, FunctionDef> FunctionsMap;
typedef std::vector<std::string> Variables;

struct Scope {
    Variables vars;
    std::string name;
};

class Scopes {
    std::vector<Scope> scopes;
    std::string indent;

public:
    void enterNew(std::string name) {
        assert(not name.empty());
        Msg::log(indent + "-> " + name);
        indent += "   ";
        auto currentOffset = 0;
        scopes.emplace_back(Scope{Variables(), name});
    }

    void leave() {
        auto &sb = scopes.back().vars;
        for (auto var = sb.rbegin(); var != sb.rend(); ++var)
            Msg::log(indent + "- " + *var);
        indent = indent.substr(0, indent.length() - 3);
        Msg::log(indent + "<- " + scopes.back().name);
        scopes.pop_back();
    }

    void addVariable(std::string name) {
        auto &currentScope = scopes.back();
        Msg::log(indent + "+ " + name);
        auto &vars = currentScope.vars;
        if (std::find(vars.cbegin(), vars.cend(), name) != vars.end()) {
            Msg::error(Msg::cite(name) + " redeclared");
        }
        vars.push_back(name);
    }

    bool find(std::string varName) {
        for (auto scopeIt = scopes.rbegin(); scopeIt != scopes.rend(); ++scopeIt) {
            auto &vars = scopeIt->vars;
            if (std::find(vars.cbegin(), vars.cend(), varName) != scopeIt->vars.end())
                return true;
        }
        return false;
    }
};

struct NameChecker {
    Scopes scopes;
    FunctionsMap functionsMap;

    void run(std::vector<Ast::Function *> program) {
        createFunctionsMap(program);
        checkFunctions(program);
    }

    void createFunctionsMap(Ast::Functions functions) {
        for (auto func: functions) {
            if (functionsMap.find(func->name) != functionsMap.end())
                Msg::error("function " + Msg::cite(func->name) + " redeclared");

            functionsMap[func->name] = FunctionDef{func->name, func->args.size()};
        }

        functionsMap["read"] = FunctionDef{"read", 0};
        functionsMap["write"] = FunctionDef{"write", 1};

        if (functionsMap.find("main") == functionsMap.end())
            Msg::error("function 'main' not found");
    }

    void checkFunctions(Ast::Functions functions) {
        for (auto func: functions)
            checkFunction(func);
    }

    void checkFunction(Ast::Function * func) {
        scopes.enterNew(func->name);
        for (auto arg: func->args)
            scopes.addVariable(arg);
        checkStatements(func->statements);
        scopes.leave();
    }

    void checkStatements(Ast::Statements stmts) {
        for (auto stmt: stmts)
            switch (stmt->type()) {
                case Ast::NodeType::VariableDecl:
                    checkVariableDecl(
                        Ast::cast<Ast::NodeType::VariableDecl>(stmt));
                    break;
                case Ast::NodeType::IfStatement:
                    checkIfStatement(
                        Ast::cast<Ast::NodeType::IfStatement>(stmt));
                    break;
                case Ast::NodeType::WhileStatement:
                    checkWhileStatement(
                        Ast::cast<Ast::NodeType::WhileStatement>(stmt));
                    break;
                case Ast::NodeType::ReturnStatement:
                    checkReturnStatement(
                        Ast::cast<Ast::NodeType::ReturnStatement>(stmt));
                    break;
                case Ast::NodeType::Assignment:
                    checkAssignment(
                        Ast::cast<Ast::NodeType::Assignment>(stmt));
                    break;
                case Ast::NodeType::FunctionCall:
                    checkFunctionCall(
                        Ast::cast<Ast::NodeType::FunctionCall>(stmt));
                    break;
                default:
                    break;
            }
    }

    void checkVariableDecl(Ast::VariableDecl * varDecl) {
        checkAssignment(varDecl->expr);
        scopes.addVariable(varDecl->name);
    }

    void checkIfStatement(Ast::IfStatement * ifStatement) {
        checkExpression(ifStatement->expr);
        scopes.enterNew("<if>");
        checkStatements(ifStatement->statements);
        scopes.leave();
    }

    void checkWhileStatement(Ast::WhileStatement * whileStatement) {
        checkExpression(whileStatement->expr);
        scopes.enterNew("<while>");
        checkStatements(whileStatement->statements);
        scopes.leave();
    }

    void checkReturnStatement(Ast::ReturnStatement * returnStatement) {
        checkExpression(returnStatement->expr);
    }

    void checkAssignment(Ast::Assignment * assignment) {
        // TODO: assignment->name
        checkAssignment(assignment->expr);
    }

    void checkAssignment(Ast::ExpressionBase * expr) {
        checkExpression(expr);
    }

    void checkFunctionCall(Ast::FunctionCall * functionCall) {
        auto name = functionCall->name;
        if (functionsMap.find(name) == functionsMap.end())
            Msg::error("function " + Msg::cite(name) + " not found");
        auto funcDef = functionsMap[name];
        if (funcDef.numberOfArgs != functionCall->args.size())
            Msg::error("function " + Msg::cite(name) + " called with wrong number of arguments");

        for (auto arg: functionCall->args)
            if (not scopes.find(arg))
                Msg::error(Msg::cite(arg) + " not found");
    }

    void checkExpression(Ast::ExpressionBase *expr) {
        switch (expr->type()) {
            case Ast::NodeType::ExpressionAtom:
                checkExpressionAtom(
                    Ast::cast<Ast::NodeType::ExpressionAtom>(expr));
                break;
            case Ast::NodeType::ExpressionBinary:
                checkExpressionBinary(
                    Ast::cast<Ast::NodeType::ExpressionBinary>(expr));
                break;
            default:
                break;
        }
    }

    void checkExpressionAtom(Ast::ExpressionAtom *expr) {
        switch (expr->node->type()) {
            case Ast::NodeType::Identifier:
                checkIdentifier(
                    Ast::cast<Ast::NodeType::Identifier>(expr->node)->val);
                break;
            case Ast::NodeType::ExpressionAtom:
            case Ast::NodeType::ExpressionBinary:
                checkExpression(
                    Ast::cast<Ast::NodeType::ExpressionBase>(expr->node));
                break;
            case Ast::NodeType::FunctionCall:
                checkFunctionCall(
                    Ast::cast<Ast::NodeType::FunctionCall>(expr->node));
                break;
            default:
                break;

        }
    }

    void checkExpressionBinary(Ast::ExpressionBinary *expr) {
        checkExpression(expr->lhs);
        checkExpression(expr->rhs);
    }

    void checkIdentifier(std::string name) {
        if (not scopes.find(name))
            Msg::error(Msg::cite(name) + " not found");
    }
};

} // namespace Checks

