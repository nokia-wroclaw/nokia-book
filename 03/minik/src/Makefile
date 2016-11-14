CXX=g++
CXXFLAGS=--std=c++11 -Wextra --pedantic -g -O0 -I.
CC=gcc
CFLAGS=-m32
LEXER=ast/minik_lexer.c
PARSER=ast/minik_parser.cpp
PARSERH=ast/minik_parser.hpp
BISON_DEBUG=bison.out
SRCS=\
	${PARSER} ${LEXER} \
	analysis/control_flow_graph.cpp\
	analysis/detecting_natural_loops.cpp\
	analysis/dominators.cpp\
	analysis/live_variables.cpp\
	utils/headers.cpp\
	ir/ir_codegen.cpp\
	ir/ir.cpp\
	ir/ir_printer.cpp\
	ir/symbol_table.cpp\
	main.cpp\
	register_allocator/coloring.cpp\
	register_allocator/interference_graph.cpp\
	register_allocator/spill_costs.cpp\
	register_allocator/spilling.cpp\
	register_allocator/subsumption.cpp\
	target_x86/x86_assembler.cpp\
	target_x86/x86_conv.cpp\
	target_x86/x86_register_pinning.cpp\
	target_x86/x86_simplify.cpp\
	transform.cpp\
	translate.cpp\

OBJS=${SRCS:%.cpp=%.o}
DEPS=${SRCS:%.cpp=%.o.dep}
MINIK=./minik

all: ${OBJS}
	${CXX} ${CXXFLAGS} -o ${MINIK} ${OBJS}
	${CC} ${CFLAGS} -c -o runtime/minikrt0.o runtime/minikrt0.c

${OBJS}: ${LEXER}

${LEXER}: ${PARSER}
	flex -o ${LEXER} ast/minik.l

${PARSER}:
	bison -d -o ${PARSER} ast/minik.yc

parser_debug:
	bison --report=all --report-file=${BISON_DEBUG} -d -o ast/minik_parser.cpp ast/minik.yc

depend:
	

clean:
	rm -f ${MINIK} *.o *.dep ${MINIK} ${BISON_DEBUG} ${LEXER} ${PARSER} ${PARSERH} ${OBJS} ${DEPS} runtime/minikrt0.o

.cpp.o: $<
	${CXX} -MMD -MP -MF $@.dep  -c ${CXXFLAGS} -o $@ $<
-include *.dep
