CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++17

main: main.cpp lex
	$(CXX) $(CXXFLAGS) main.cpp -o test

lex: lex.l
	flex -o lex.yy.cpp lex.l
	g++ -std=c++17 lex.yy.cpp -o scanner

clean:
	rm -f test scanner scanner_out.txt

.PHONY: clean
