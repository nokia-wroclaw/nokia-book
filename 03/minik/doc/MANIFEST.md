Manifest
========

There is short description of headers from the directory `src`.

Pipeline
--------
* `translate.hpp` -
  pipeline for whole compilation process
* `transform.hpp` -
  intermediate pipeline for procedure body


Intermediate representation
---------------------------

* !`three_address_code.hpp` - intermediate language
* !`three_address_code_printer.hpp` - printers
* `tac_codegen.hpp` - codegenerator version for IR

Control and Data flow analysis
-------------------------------

* `control_flow_graph.hpp` -
  representation and building of CFG
* `data_flow_analysis.hpp` -
  framework for Data Flow Analysis
* `live_variables.hpp` -
  live variable analysis driver for DFA framework
* `dominators.hpp` -
  dominators analysis driver for DFA framework
* `detecting_natural_loops.hpp` -
  detecting natural loops in CFG using dominators

Register allocation
--------------------

* `inference_graph.hpp` -
  representation and building of IG
* `coloring.hpp` -
  graph coloring heuristics
* `spill_costs.hpp` -
  spill costs
* `spilling.hpp` -
  register spilling
* `subsumption.hpp` -
  register coalescencing

X86 backend
------------

* `x86_assembler.hpp`
* `x86_register_pinning.hpp`
* !`x86_simplify.hpp`
* `x86_conv.hpp`

Utils
-----
* `graph.hpp` - simple graph representation

