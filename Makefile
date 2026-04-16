# This code is written by Kylie Roenigk and Angie Bragg
CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++17

all: main

main: main.cpp tree_builder.tab.cpp
	$(CXX) $(CXXFLAGS) main.cpp tree_builder.tab.cpp -o main

tree_builder.tab.cpp tree_builder.tab.hpp: tree_builder.y lex.yy.c
	bison -d -o tree_builder.tab.cpp tree_builder.y

lex.yy.c: treeBuilder.l tree_builder.tab.hpp
	flex -o lex.yy.c treeBuilder.l

parser: parser_main.cpp tree_builder.tab.cpp
	$(CXX) $(CXXFLAGS) parser_main.cpp tree_builder.tab.cpp -o parser

clean:
	rm -f test main parser scanner_out.txt tokenizedCheck.txt tokens.txt \
		lex.yy.cpp lex.yy.c tree_builder.tab.cpp tree_builder.tab.hpp tree_builder.tab.c tree_builder.tab.h

.PHONY: all clean parser
