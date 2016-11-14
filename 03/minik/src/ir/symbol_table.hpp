#pragma once
#include "ast/nodes.hpp"
#include "ir.hpp"

#include <unordered_map>

typedef std::unordered_map<Ir::Argument, int> LocalStore;

struct FunctionDef {
    std::string name;
    std::string symName;
    Ast::Arguments arguments;
    bool predefined;
    Ir::Argument id;
    Ir::Block body;
    size_t frameSize;
    std::set<Ir::Argument> modifiedRegs;
    LocalStore localStore;
};

class SymbolTable {
    typedef std::unordered_map<std::string, FunctionDef> FunctionsMap;
public:
    FunctionsMap functionsMap;
    std::unordered_map<Ir::Argument, std::string> argumentToFunction;

    void addFunction(std::string name, FunctionDef function);
    void mapFunctionToArgument(const std::string& name, Ir::Argument arg);
    void addBodyToFunction(const std::string& name, const Ir::Block& body);
    const FunctionDef &getFunction(Ir::Argument arg) const;
    const FunctionDef &getFunction(const std::string &name) const;
};

SymbolTable createSymbolTable(Ast::Functions functions);

