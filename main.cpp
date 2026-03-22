#include <iostream>
#include <fstream>


using namespace std;

int main() {
    /// Open tokens file
    ifstream infileTokens("tokens.txt");
    if (!infileTokens) {
        cerr << "Failed to open tokens.txt\n";
        return 1;
    }
    cout << "Tokens.txt opening...." << endl;

    infileTokens.close();
    return 0;
}
