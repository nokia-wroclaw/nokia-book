#include "ast/nodes.hpp"
#include "ast/checks.hpp"
#include "analysis/control_flow_graph.hpp"
#include "register_allocator/interference_graph.hpp"
#include "analysis/live_variables.hpp"
#include "ir/ir.hpp"
#include "ir/ir_codegen.hpp"
#include "ir/ir_printer.hpp"
#include "target_x86/x86_assembler.hpp"
#include "target_x86/x86_simplify.hpp"
#include "register_allocator/subsumption.hpp"
#include "register_allocator/coloring.hpp"
#include "transform.hpp"
#include "target_x86/x86_conv.hpp"
#include "target_x86/x86_register_pinning.hpp"
#include "utils/headers.hpp"
#include <iostream>

extern Ast::Functions program;


void translate()
{
    Ir::CodeGen irCg;
    auto st = createSymbolTable(program);

    pass("codegen");
    irCg.translate(program, st);
    std::cerr << to_string(st) << "\n";

    pass("x86 idiosyncrasy");
    x86_idiosyncrasy(st);

    pass("transform");
    transform(st);

    pass("transformation summary");
    std::cerr << to_string(st) << "\n";

    pass("x86 pin");
    x86_pin(st);

    pass("x86 pin summary");
    std::cerr << to_string(st) << "\n";

    pass("x86 store operations");
    x86_store_operations(st);

    pass("x86 unfold call/return");
    x86_unfold_callreturn(st);

    pass("x86 default return");
    x86_default_return(st);

    pass("x86 conv");
    x86_conv(st);

    pass("x86 conv summary");
    std::cerr << to_string(st) << "\n";

    pass("x86 assembler");
    Assembler x86asm;
    std::cout << x86asm.compile(st) << "\n";
}
