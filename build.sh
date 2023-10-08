#! /bin/bash
bison --header -o parser.c parser.y 
flex --header-file=lexer.yy.h tokens.l
g++ -c `llvm-config --cxxflags --ldflags --system-libs --libs core` -o parser.o parser.c


