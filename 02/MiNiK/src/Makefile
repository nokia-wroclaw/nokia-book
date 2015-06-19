CXX=g++-4.9
CXXFLAGS=--std=c++11 -Wextra --pedantic -g -O0
CC=gcc
CFLAGS=-m32
LEXER=minik_lexer.c
PARSER=minik_parser.cpp
PARSERH=minik_parser.hpp
BISON_DEBUG=bison.out
SRCS=${PARSER} ${LEXER} main.cpp
MINIK=./minik

all: parser lexer
	${CXX} ${CXXFLAGS} -o ${MINIK} ${SRCS}
	${CC} ${CFLAGS} -c -o minikrt0.o minikrt0.c

lexer:
	flex -o ${LEXER} minik.l

parser:
	bison -d -o ${PARSER} minik.yc

parser_debug:
	bison --report=all --report-file=${BISON_DEBUG} -d -o minik_parser.cpp minik.yc

clean:
	rm -f ${MINIK} *.o ${MINIK} ${BISON_DEBUG} ${LEXER} ${PARSER} ${PARSERH}

