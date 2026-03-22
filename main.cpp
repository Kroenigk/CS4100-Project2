#include <iostream>
#include <fstream>
#include <string>


using namespace std;

int main() {
    /// Open tokens file
    ifstream infileTokens("tokens.txt");
    if (!infileTokens) {
        cerr << "Failed to open tokens.txt\n";
        return 1;
    }
    cout << "Tokens.txt opening...." << endl;

    ///Print tokens
    string line;
    while (getline(infileTokens, line)) {
        cout << line << endl; 
    }

    infileTokens.close();
    return 0;
}
