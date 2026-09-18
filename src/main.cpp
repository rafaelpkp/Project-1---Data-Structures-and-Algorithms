#include <iostream>
#include <string>

#include "AVL.h"
#include "CommandParser.h"

using namespace std;

int main() {
    AVL tree;

    string line;
    if (!getline(cin, line)) {
        return 0;
    }
    int numCommands = stoi(line);

    for (int i = 0; i < numCommands && getline(cin, line); i++) {
        cout << executeCommand(tree, line) << endl;
    }
    return 0;
}
