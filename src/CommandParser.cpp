#include "CommandParser.h"

#include <cctype>
#include <iomanip>
#include <sstream>
#include <vector>

namespace {

const std::string SUCCESS = "successful";
const std::string FAIL = "unsuccessful";

std::string formatID(int id) {
    std::ostringstream out;
    out << std::setw(8) << std::setfill('0') << id;
    return out.str();
}

std::string joinNames(const std::vector<std::string>& names) {
    std::string result;
    for (size_t i = 0; i < names.size(); i++) {
        if (i > 0) {
            result += ", ";
        }
        result += names[i];
    }
    return result;
}

// Reads a "quoted string" starting at pos. Returns false if it isn't there.
bool readQuoted(const std::string& line, size_t& pos, std::string& out) {
    while (pos < line.size() && line[pos] == ' ') {
        pos++;
    }
    if (pos >= line.size() || line[pos] != '"') {
        return false;
    }
    size_t close = line.find('"', pos + 1);
    if (close == std::string::npos) {
        return false;
    }
    out = line.substr(pos + 1, close - pos - 1);
    pos = close + 1;
    return true;
}

// Reads the rest of the line as a single token. Returns false if it is empty
// or contains more than one word.
bool readLastToken(const std::string& line, size_t pos, std::string& out) {
    std::istringstream in(line.substr(pos));
    std::string extra;
    if (!(in >> out) || (in >> extra)) {
        return false;
    }
    return true;
}

bool isNonNegativeInt(const std::string& s) {
    if (s.empty() || s.size() > 9) {
        return false;
    }
    for (char c : s) {
        if (!std::isdigit(static_cast<unsigned char>(c))) {
            return false;
        }
    }
    return true;
}

}  // namespace

bool isValidName(const std::string& name) {
    if (name.empty()) {
        return false;
    }
    for (char c : name) {
        if (!std::isalpha(static_cast<unsigned char>(c)) && c != ' ') {
            return false;
        }
    }
    return true;
}

bool isValidID(const std::string& id) {
    if (id.size() != 8) {
        return false;
    }
    for (char c : id) {
        if (!std::isdigit(static_cast<unsigned char>(c))) {
            return false;
        }
    }
    return true;
}

std::string executeCommand(AVL& tree, const std::string& line) {
    std::istringstream in(line);
    std::string command;
    if (!(in >> command)) {
        return FAIL;
    }
    size_t pos = line.find(command) + command.size();

    if (command == "insert") {
        std::string name, id;
        if (!readQuoted(line, pos, name) || !readLastToken(line, pos, id)) {
            return FAIL;
        }
        if (!isValidName(name) || !isValidID(id)) {
            return FAIL;
        }
        return tree.insert(name, std::stoi(id)) ? SUCCESS : FAIL;
    }

    if (command == "remove") {
        std::string id;
        if (!readLastToken(line, pos, id) || !isValidID(id)) {
            return FAIL;
        }
        return tree.remove(std::stoi(id)) ? SUCCESS : FAIL;
    }

    if (command == "search") {
        std::string name;
        size_t quotePos = pos;
        if (readQuoted(line, quotePos, name)) {
            std::string extra;
            if (readLastToken(line, quotePos, extra) || !isValidName(name)) {
                return FAIL;
            }
            std::vector<int> ids = tree.searchName(name);
            if (ids.empty()) {
                return FAIL;
            }
            std::string result;
            for (size_t i = 0; i < ids.size(); i++) {
                if (i > 0) {
                    result += "\n";
                }
                result += formatID(ids[i]);
            }
            return result;
        }

        std::string id;
        if (!readLastToken(line, pos, id) || !isValidID(id)) {
            return FAIL;
        }
        std::string found;
        return tree.searchID(std::stoi(id), found) ? found : FAIL;
    }

    if (command == "removeInorder") {
        std::string n;
        if (!readLastToken(line, pos, n) || !isNonNegativeInt(n)) {
            return FAIL;
        }
        return tree.removeInorder(std::stoi(n)) ? SUCCESS : FAIL;
    }

    // the remaining commands take no arguments
    std::string extra;
    if (in >> extra) {
        return FAIL;
    }
    if (command == "printInorder") {
        return joinNames(tree.inorderNames());
    }
    if (command == "printPreorder") {
        return joinNames(tree.preorderNames());
    }
    if (command == "printPostorder") {
        return joinNames(tree.postorderNames());
    }
    if (command == "printLevelCount") {
        return std::to_string(tree.levelCount());
    }

    return FAIL;
}
