#include "register_allocator/coloring.hpp"
#include "analysis/control_flow_graph.hpp"
#include "analysis/detecting_natural_loops.hpp"
#include "analysis/dominators.hpp"
#include "register_allocator/spilling.hpp"
#include "utils/headers.hpp"
#include "register_allocator/interference_graph.hpp"
#include "analysis/live_variables.hpp"
#include "register_allocator/spill_costs.hpp"
#include "register_allocator/subsumption.hpp"
#include "ir/ir_codegen.hpp"
#include "ir/ir.hpp"
#include "ir/ir_printer.hpp"
#include "transform.hpp"
#include "target_x86/x86_assembler.hpp"
#include "target_x86/x86_conv.hpp"
#include "target_x86/x86_simplify.hpp"
#include <iostream>

namespace {

void title(const FunctionDef &def)
{
    std::string title1;
    title1 += "function: ";
    title1 += def.name;
    title1 += "/";
    title1 += std::to_string(def.arguments.size());

    std::string title2;
    title2 += "symbol: ";
    title2 += def.symName;

    subpass(title1, title2);
}

void header(const FunctionDef &def, const std::string &n)
{
    step(def.name, "step: " + n);
}

void welcome_pass(FunctionDef &def)
{
    header(def, "input");
    std::cerr << to_string(def.body) << "\n";

    header(def, "initial control flow graph");
    auto cfg = buildControlFlowGraph(def.body);
    cfg.dump(std::cerr, true);

    header(def, "initial live variables statical analysis");
    auto result = LiveVariablesDataFlowAnalysis::analyse(cfg);
    LiveVariablesDataFlowAnalysis::dump(std::cerr, cfg, result);

    header(def, "initial interference graph");
    auto ig = buildInferenceGraph(result, cfg);
    ig.dump(std::cerr);

}

void spill(const ControlFlowGraph &cfg, const std::set<Ir::Argument> &potentialSpills, FunctionDef &def)
{
    header(def, "dominators");
    auto dominators = DominatorsDataFlowAnalysis::analyse(cfg);
    DominatorsDataFlowAnalysis::dump(std::cerr, cfg, dominators);

    header(def, "natural loops");
    auto mapping = detectNaturalLoop(cfg, dominators);

    header(def, "spill costs");
    auto costs = computeSpillCosts(potentialSpills, def.localStore, def.body, mapping);
    for (auto p : costs) {
        std::cerr << "cost for " << to_string(p.first) << " is " << p.second << "\n";
    }

    header(def, "spilling");
    def.body = insert_spillcode(def.localStore, def.body, costs);

    std::cerr << "\n" << to_string(def.body) << "\n";

    header(def, "local store");

    for (auto p : def.localStore) {
        std::cerr << "register " << to_string(p.first) << " is local variable " << p.second << "\n";
    }
}

bool allocate_registers(FunctionDef &def)
{
    header(def, "subsumption");
    def.body = subsumption(def.body);
    std::cerr << "\n" << to_string(def.body) << "\n";

    header(def, "control flow graph after subsumption");
    auto cfg = buildControlFlowGraph(def.body);
    cfg.dump(std::cerr, true);

    header(def, "live variables statical analysis after subsumption");
    auto result = LiveVariablesDataFlowAnalysis::analyse(cfg);
    LiveVariablesDataFlowAnalysis::dump(std::cerr, cfg, result);

    header(def, "interference graph after subsumption");
    auto ig = buildInferenceGraph(result, cfg);
    ig.dump(std::cerr);

    header(def, "register allocation");
    auto coloringResult = linear_color(def.body, 6);
    if (not coloringResult.valid) {
        std::cerr << "FAILED: need spill\n";
        spill(cfg, coloringResult.potentialSpills, def);
        return false;
    }

    std::cerr << "REGISTERS ALLOCATED\n";
    def.body = applyColors(coloringResult.colors, def.body);



    return true;
}


void transform(FunctionDef &def)
{
    title(def);
    welcome_pass(def);
    while (not allocate_registers(def));

    header(def, "output");
    std::cerr << to_string(def.body) << "\n";

}

} // end of anonymous namespace

void transform(SymbolTable &symbolTable)
{
    for (auto &p : symbolTable.functionsMap) {
        if (not p.second.predefined) transform(p.second);
    }
}


