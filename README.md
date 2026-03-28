# CS4100-Project2
This code is written by Kylie Roenigk and Angie Bragg
## Install Depencies
```bash
sudo apt update
sudo apt install flex bison gcc make
```

## Testing the Lexer
```bash
flex -o lex.yy.cpp treeBuilder.l
g++ -std=c++17 lex.yy.cpp -o scanner
./scanner < test.txt
```

## Running Instructions
```bash
make clean
make
./test.sh Example
```
This will compile the lex file and then run it on the example file. The main function in main.cpp will then check the tokenized output from lex compared to what tokens are expected.
