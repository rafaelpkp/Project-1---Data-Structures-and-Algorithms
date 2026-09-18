// Name: Rafael Penhas
// UFID: 40840194

#include <algorithm>
#include <iostream>
#include <cstdlib>
#include <set>
#include <string>
#include <vector>

#include "catch/catch_amalgamated.hpp"
#include "../src/AVL.h"
#include "../src/CommandParser.h"

using namespace std;

// Test 1: at least five commands that should print "unsuccessful"
TEST_CASE("Incorrect commands print unsuccessful", "[unsuccessful]") {
	AVL tree;

	// name contains digits
	REQUIRE(executeCommand(tree, "insert \"A11y\" 45679999") == "unsuccessful");
	// ID is too short (7 digits)
	REQUIRE(executeCommand(tree, "insert \"Adam\" 1234567") == "unsuccessful");
	// ID is too long (9 digits)
	REQUIRE(executeCommand(tree, "insert \"Adam\" 123456789") == "unsuccessful");
	// ID contains a letter
	REQUIRE(executeCommand(tree, "insert \"Adam\" 1234567a") == "unsuccessful");

	// duplicate ID
	REQUIRE(executeCommand(tree, "insert \"Adam\" 12345678") == "successful");
	REQUIRE(executeCommand(tree, "insert \"Brian\" 12345678") == "unsuccessful");
	// remove / search an ID that is not in the tree
	REQUIRE(executeCommand(tree, "remove 87654321") == "unsuccessful");
	REQUIRE(executeCommand(tree, "search 87654321") == "unsuccessful");
	REQUIRE(executeCommand(tree, "search \"Brian\"") == "unsuccessful");
	// removeInorder index out of range (only 1 node, so index 1 is invalid)
	REQUIRE(executeCommand(tree, "removeInorder 1") == "unsuccessful");
	REQUIRE(tree.size() == 1);
}

// Test 2: insert command and all four rotation cases
TEST_CASE("Insert and all four rotation cases", "[insert][rotation]") {
	AVL tree;

	SECTION("Insert with no rotation") {
		REQUIRE(executeCommand(tree, "insert \"Brandon\" 20000000") == "successful");
		REQUIRE(executeCommand(tree, "insert \"Alex\" 10000000") == "successful");
		REQUIRE(executeCommand(tree, "insert \"Carl\" 30000000") == "successful");
		REQUIRE(tree.preorderIDs() == vector<int>{20000000, 10000000, 30000000});
	}

	SECTION("Left-left case (right rotation)") {
		// 30 -> 20 -> 10 all going left
		tree.insert("Carl", 30000000);
		tree.insert("Brandon", 20000000);
		tree.insert("Alex", 10000000);
		REQUIRE(tree.preorderIDs() == vector<int>{20000000, 10000000, 30000000});
		REQUIRE(tree.levelCount() == 2);
	}

	SECTION("Right-right case (left rotation)") {
		// 10 -> 20 -> 30 all going right
		tree.insert("Alex", 10000000);
		tree.insert("Brandon", 20000000);
		REQUIRE(tree.inorderIDs() == vector<int>{10000000, 20000000, 30000000});
		REQUIRE(tree.levelCount() == 2);
		REQUIRE(tree.isBalanced());
	}

	SECTION("Left-right case (left-right rotation)") {
		// 30, then 10 on the left, then 20 on the right of 10
		tree.insert("Carl", 30000000);
		tree.insert("Alex", 10000000);
		REQUIRE(tree.inorderIDs() == vector<int>{10000000, 20000000, 30000000});
		REQUIRE(tree.levelCount() == 2);
		REQUIRE(tree.isBalanced());
	}

	SECTION("Right-left case (right-left rotation)") {
		// 10, then 30 on the right, then 20 on the left of 30
		tree.insert("Alex", 10000000);
		tree.insert("Carl", 30000000);
		REQUIRE(tree.inorderIDs() == vector<int>{10000000, 20000000, 30000000});
		REQUIRE(tree.levelCount() == 2);
		REQUIRE(tree.isBalanced());
	}
}

// Test 3: insert 100 nodes, remove 10 random ones, check inorder
TEST_CASE("Insert 100 nodes, remove 10, check inorder", "[insert][remove]") {
	AVL tree;
	vector<int> expectedOutput;

	// insert 100 unique random 8-digit IDs
	set<int> used;
	while (expectedOutput.size() < 100) {
		int id = 10000000 + rand() % 90000000;
		if (used.count(id)) {
			continue;
		}
		used.insert(id);
		expectedOutput.push_back(id);
		REQUIRE(executeCommand(tree, "insert \"Student\" " + to_string(id)) == "successful");
	} 

	sort(expectedOutput.begin(), expectedOutput.end());
	vector<int> actualOutput = tree.inorderIDs();
	REQUIRE(tree.size() == 100);
	REQUIRE(expectedOutput.size() == actualOutput.size());
	REQUIRE(actualOutput == expectedOutput);
	REQUIRE(tree.isBalanced());

	// remove 10 random nodes that are in the tree
	for (int i = 0; i < 10; i++) {
		int index = rand() % expectedOutput.size();
		int id = expectedOutput[index];
		REQUIRE(executeCommand(tree, "remove " + to_string(id)) == "successful");
		expectedOutput.erase(expectedOutput.begin() + index);
	}

	actualOutput = tree.inorderIDs();
	REQUIRE(tree.size() == 90);
	REQUIRE(expectedOutput.size() == actualOutput.size());
	for (size_t i = 0; i < expectedOutput.size(); i++) {
		REQUIRE(actualOutput[i] == expectedOutput[i]);
	}
}
