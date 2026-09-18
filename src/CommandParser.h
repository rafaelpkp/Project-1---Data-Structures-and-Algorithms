#pragma once

#include <string>

#include "AVL.h"

// Parses one line of input (e.g. insert "Adam" 12345678), runs it on the tree,
// and returns what should be printed (lines separated by '\n', no trailing newline).
// Returning a string instead of printing lets the tests check the exact output.
std::string executeCommand(AVL& tree, const std::string& line);

bool isValidName(const std::string& name);
bool isValidID(const std::string& id);
