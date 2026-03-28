CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++17

main: main.cpp treeBuilder
	$(CXX) $(CXXFLAGS) main.cpp -o main

treeBuilder: treeBuilder.l
	flex -o lex.yy.cpp treeBuilder.l
	g++ -std=c++17 lex.yy.cpp -o scanner

clean:
	rm -f test scanner scanner_out.txt tokenizedCheck.txt tokens.txt lex.yy.cpp

.PHONY: clean
