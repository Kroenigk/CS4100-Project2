#include <iostream>
#include <fstream>
#include <string>


using namespace std;

bool compareFiles(const string& output, const string& expected)
{
    ifstream lexOutput(output);
    ifstream expectedTokens(expected);

    if (!lexOutput || !expectedTokens) return false;

    string lexLine, expectedLine;
    int lineNum = 1;

    while(true)
    {
        bool gotLex = static_cast<bool>(getline(lexOutput, lexLine));
        bool gotExpected = static_cast<bool>(getline(expectedTokens, expectedLine));

        // Both files should end at the same time.
        if(!gotLex && !gotExpected)
        {
            return true;
        }

        // If one file has extra lines, they are not equal.
        if(gotLex != gotExpected)
        {
            cout << "Difference at Line Number " << lineNum << endl;
            cout << "Files differ in length." << endl;
            return false;
        }

        if(lexLine != expectedLine)
        {
            cout << "Difference at Line Number " << lineNum << endl;
            cout << "Lex Line: " << lexLine << endl;
            cout << "Expected Line: " << expectedLine << endl;
            return false;
        }
        ++lineNum;
    }
}

int main() {
    if (compareFiles("scanner_out.txt", "results.txt"))
    {
        cout << "Tokens from Lex match what was expected" << endl;
    }
    return 0;
}
