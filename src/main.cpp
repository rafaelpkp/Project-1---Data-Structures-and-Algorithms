#include <iostream>
#include <string>
#include <sstream>

#include "AVL.h"
#include "CommandParser.h"

using namespace std;

int main() {
    AVL tree;

    // Get info from line
    string line;

    if (!getline(cin, line)) {
        return 0;
    }

    getline(cin, line);
    
    int numCommands = stoi(line);

    istringstream newCin(line);

    for (int i = 0; i < numCommands; i++) {
        getline(cin, line);
        istringstream newCin(line);
        string command; newCin >> command;

        if(command == "insert"){
            insertHelper()
        }



        // cout << executeCommand(tree, line) << endl;
        // tree.insert("Jackie", 0000000);
    }

            string command; newCin >> command;

    return 0;
} 
