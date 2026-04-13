#include <iostream>

using namespace std;

extern int parse_input_file(const char *path);

int main(int argc, char **argv) {
    if (argc > 2) {
        cerr << "Usage: " << argv[0] << " [input-file]" << endl;
        return 1;
    }

    const char *path = NULL;
    if (argc == 2) {
        path = argv[1];
    }

    return parse_input_file(path);
}
