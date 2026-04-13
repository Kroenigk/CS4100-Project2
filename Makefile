# This code is written by Kylie Roenigk and Angie Bragg
CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++17

all: main scanner parser

main: main.cpp tree_builder.tab.cpp lex.yy.c
	$(CXX) $(CXXFLAGS) main.cpp tree_builder.tab.cpp -o main

scanner: treeBuilder.l
	flex -o lex.yy.cpp treeBuilder.l
	$(CXX) $(CXXFLAGS) lex.yy.cpp -o scanner

tree_builder.tab.cpp tree_builder.tab.hpp: tree_builder.y
	bison -d -o tree_builder.tab.cpp tree_builder.y

lex.yy.c: treeBuilder.l
	flex -o lex.yy.c treeBuilder.l

parser: parser_main.cpp tree_builder.tab.cpp lex.yy.c
	$(CXX) $(CXXFLAGS) parser_main.cpp tree_builder.tab.cpp -o parser

clean:
	rm -f test main scanner parser scanner_out.txt tokenizedCheck.txt tokens.txt \
		lex.yy.cpp lex.yy.c tree_builder.tab.cpp tree_builder.tab.hpp tree_builder.tab.c tree_builder.tab.h

.PHONY: all clean
