#include "symbol_table.hpp"

void SymbolTable::addFunction(std::string name, FunctionDef function) {
    functionsMap[name] = function;
}

void SymbolTable::mapFunctionToArgument(const std::string& name, Ir::Argument arg) {
    argumentToFunction[arg] = name;
}

void SymbolTable::addBodyToFunction(const std::string& name, const Ir::Block& body) {
    functionsMap[name].body = body;
}

const FunctionDef &SymbolTable::getFunction(Ir::Argument arg) const {
    return functionsMap.at(argumentToFunction.at(arg));
}

const FunctionDef &SymbolTable::getFunction(const std::string &name) const {
    return functionsMap.at(name);
}

SymbolTable createSymbolTable(Ast::Functions functions) {
    SymbolTable st;
    int functionCounter = 1;

    auto getNextFunctionId = [&] () {
      return Ir::Argument{ Ir::ArgumentType::FunctionNumber, functionCounter++ };
    };

    {
        auto functionId = getNextFunctionId();
        st.addFunction("read", FunctionDef{"read", "__MINIK_read", Ast::Arguments{}, true, functionId, {}, 0, {}, {}});
        st.mapFunctionToArgument("read", functionId);
    }

    {
        auto functionId = getNextFunctionId();
        st.addFunction("write", FunctionDef{"write", "__MINIK_write", Ast::Arguments{"dummy"}, true, functionId, {}, 0, {}, {}});
        st.mapFunctionToArgument("write", functionId);
    }

    for (auto func: functions) {
        auto functionId = getNextFunctionId();
        auto functionDef =
            FunctionDef{
                func->name,
                "__MINIK_" + func->name,
                func->args,
                false,
                functionId,
                {},
                0,
                {},
                {}};

       st.addFunction(func->name, functionDef);
       st.mapFunctionToArgument(func->name, functionId);
    }

    return st;
}

