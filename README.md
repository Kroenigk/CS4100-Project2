# CS4100-Project2
This code is written by Kylie Roenigk and Angie Bragg
## Install Depencies
```bash
sudo apt update
sudo apt install flex bison gcc make
```

## Running Instructions
```bash
make clean
make
./main Example/example.txt
```
This will compile the lex file, yacc file, and then run it on the example file. The main function in main.cpp will then print out the tree from the example file to the terminal
