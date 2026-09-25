#include <iostream>
#include <string>

#include "AVL.h"
#include "CommandParser.h"

using namespace std;

int main() {
    AVL tree;

    // First line is the number of commands that follow
    string line;
    if (!getline(cin, line)) {
        return 0;
    }

    int numCommands = 0;
    try {
        numCommands = stoi(line);
    } catch (...) {
        return 0;
    }

    // Each command is parsed and run by executeCommand, which returns the text to print
    for (int i = 0; i < numCommands && getline(cin, line); i++) {
        if (!line.empty() && line.back() == '\r') {
            line.pop_back();
        }
        cout << executeCommand(tree, line) << endl;
    }

    return 0;
}
