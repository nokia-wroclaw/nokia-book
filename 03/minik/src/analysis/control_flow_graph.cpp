#include "control_flow_graph.hpp"
#include "ir/ir_printer.hpp"

ControlFlowGraph::ControlFlowGraph()
{
    using namespace Ir;
    auto meta_entry = Instruction{InstructionType::Meta, { {ArgumentType::Meta, int(MetaTag::Entry)} }, 0};
    auto meta_exit  = Instruction{InstructionType::Meta, { {ArgumentType::Meta, int(MetaTag::Exit)} }, 0};
    isolatedEntry = graph.addVertex(meta_entry);
    isolatedExit = graph.addVertex(meta_exit);
}

void ControlFlowGraph::dump(std::ostream &os, bool printIr) const {
    os << "digraph CONTROL_FLOW_GRAPH {\n";

    for (auto v : graph) {

        os << "V" << v;
        if (printIr) {
            os << "[label=\"";
            os << to_string(graph.getVertexData(v));
            os << "\"]";
        }
        os << ";\n";
    }

    for (auto v: graph) {
        for (auto u: graph.getOutNeighbours(v)) {
            os << "V" << v << " -> V" << u << ";\n";
        }
    }

    os << "}\n";
}

void ControlFlowGraph::dumpWithIO(std::ostream &os, std::function<std::string (int)> inF,  std::function<std::string (int)> outF) const
{
    os << "digraph CONTROL_FLOW_GRAPH_WITH_INPUT_OUTPUT {\n";

    os << "node [width=3.00];\n";

    for (auto v : graph) {

        os << "V" << v;
        os << "[shape=box; fillcolor=skyblue; style=filled; label=\"";
        os << std::to_string(v);
        os << ": ";
        os << to_string(graph.getVertexData(v));
        os << "\"]";
        os << ";\n";

        os << "V" << v << "_in" ;
        os << "[shape=house; style=filled; fillcolor=yellow2; label=\"";
        os << inF(v);
        os << "\"]";
        os << ";\n";

        os << "V" << v << "_out" ;
        os << "[shape=invhouse; style=filled; fillcolor=orange; label=\"";
        os << outF(v);
        os << "\"]";
        os << ";\n";

        os << "subgraph cluster_C" << v << " {\n";
        os << "V" << v << "_in -> " << "V" << v << "[style=dashed];\n";
        os << "V" << v << " -> " << "V" << v << "_out[style=dashed];\n";
        os << "graph[style=\"filled\"; fillcolor=lightgrey; color=black];\n";
        os << "}\n";
    }

    for (auto v: graph) {
        for (auto u: graph.getOutNeighbours(v)) {
            os << "V" << v << "_out -> V" << u << "_in;\n";
        }
    }

    os << "}\n";
}

bool goesForward(Ir::InstructionType tp)
{
    using namespace Ir;
    switch (tp) {
        case InstructionType::Goto:
        case InstructionType::Return:
            return false;

        default:
            break;
    }

    return true;
}


ControlFlowGraph buildControlFlowGraph(Ir::Block &block)
{
    using namespace Ir;

    ControlFlowGraph cfg;
    ControlFlowGraph::Graph::V_t prev = cfg.isolatedEntry;
    ControlFlowGraph::Graph::V_t v = prev;
    bool fromPreviousInstruction = true;
    std::unordered_map<int, int> labels;

    auto it = block.begin();
    for (size_t i = 0; i < block.size(); i++) {
        if (it->type == InstructionType::Label) {
            auto u = cfg.graph.addVertex(*it);
            labels[it->arguments[0].content] = u;
            it->extra = u;
        }
        ++it;
    }

    it = block.begin();
    for (size_t i = 0; i < block.size(); i++) {

        if (it->type == InstructionType::Label) {
            v = labels[it->arguments[0].content];
        } else {
            v = cfg.graph.addVertex(*it);
        }
        it->extra = v;

        if (fromPreviousInstruction) {
            cfg.graph.addEdgeFromTo(prev, v);
        }

        switch (it->type) {
            case InstructionType::IfEq:
            case InstructionType::IfNeq:
            case InstructionType::IfLt:
            case InstructionType::IfLeq:
            case InstructionType::IfGt:
            case InstructionType::IfGeq:
            case InstructionType::Goto:
                cfg.graph.addEdgeFromTo(v, labels.at(it->arguments[0].content));
                break;
            case InstructionType::Return:
                cfg.graph.addEdgeFromTo(v, cfg.isolatedExit);
                break;
        }

        prev = v;
        fromPreviousInstruction = goesForward(it->type);
        ++it;

    }

    if (fromPreviousInstruction) cfg.graph.addEdgeFromTo(prev, cfg.isolatedExit);

    return cfg;
}
