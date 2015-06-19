#include "nodes.hpp"
#include "checks.hpp"
#include "codegen.hpp"

#include <iostream>

extern Ast::Functions program;
extern int yyparse();

void printSyntaxTree()
{
    for (auto st : program)
        std::cerr << st->toString();
}

void check()
{
    Checks::NameChecker nc;
    nc.run(program);
}

void translate()
{
    CodeGen cg;
    std::cout << cg.translate(program);
}

int main()
{
    yyparse();
    printSyntaxTree();
    check();
    translate();
    for (auto st : program)
        delete st;
    return 0;
}
