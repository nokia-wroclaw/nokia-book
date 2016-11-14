#include "ast/nodes.hpp"
#include "ast/checks.hpp"
#include "utils/messages.hpp"
#include "ir/ir.hpp"
#include "ir/ir_codegen.hpp"
#include "ir/ir_printer.hpp"
#include "transform.hpp"
#include "translate.hpp"
#include <iostream>

extern Ast::Functions program;
extern int yyparse();
extern "C" FILE *yyin;

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



int main(int argc, char *argv[])
{
    if (argc < 2)
        Msg::error("No input file passed");

    yyin = 0;
    yyin = fopen(argv[1], "r");
    if (not yyin)
        Msg::error(std::string("Invalid input file: ") + argv[1]);

    Msg::log(std::string("Compiling: '") + argv[1] + "'...\n");

    yyparse();
    printSyntaxTree();
    check();
    translate();
    for (auto st : program)
        delete st;
    return 0;
}
