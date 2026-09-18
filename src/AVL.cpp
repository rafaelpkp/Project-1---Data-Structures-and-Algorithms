#include "AVL.h"

#include <algorithm>
#include <cstdlib>

AVL::AVL() : root(nullptr), nodeCount(0) {}

AVL::~AVL() {
    destroy(root);
}

void AVL::destroy(Node* node) {
    if (!node) {
        return;
    }
    destroy(node->left);
    destroy(node->right);
    delete node;
}

// ---------- height / balance ----------

int AVL::height(Node* node) const {
    return node ? node->height : 0;
}

void AVL::updateHeight(Node* node) {
    node->height = 1 + std::max(height(node->left), height(node->right));
}

int AVL::balanceFactor(Node* node) const {
    return node ? height(node->left) - height(node->right) : 0;
}

// ---------- rotations ----------

AVL::Node* AVL::rotateLeft(Node* node) {
    Node* newRoot = node->right;
    node->right = newRoot->left;
    newRoot->left = node;
    updateHeight(node);
    updateHeight(newRoot);
    return newRoot;
}

AVL::Node* AVL::rotateRight(Node* node) {
    Node* newRoot = node->left;
    node->left = newRoot->right;
    newRoot->right = node;
    updateHeight(node);
    updateHeight(newRoot);
    return newRoot;
}

AVL::Node* AVL::rotateLeftRight(Node* node) {
    node->left = rotateLeft(node->left);
    return rotateRight(node);
}

AVL::Node* AVL::rotateRightLeft(Node* node) {
    node->right = rotateRight(node->right);
    return rotateLeft(node);
}

// ---------- insert ----------

AVL::Node* AVL::insertHelper(Node* node, const std::string& name, int id, bool& inserted) {
    if (!node) {
        inserted = true;
        return new Node(name, id);
    }

    if (id < node->id) {
        node->left = insertHelper(node->left, name, id, inserted);
    }
    else if (id > node->id) {
        node->right = insertHelper(node->right, name, id, inserted);
    }
    else {
        // duplicate ID
        return node;
    }

    updateHeight(node);
    int balance = balanceFactor(node);

    if (balance > 1) {
        if (id < node->left->id) {
            return rotateRight(node);        // left-left
        }
        return rotateLeftRight(node);        // left-right
    }
    if (balance < -1) {
        if (id > node->right->id) {
            return rotateLeft(node);         // right-right
        }
        return rotateRightLeft(node);        // right-left
    }
    return node;
}

bool AVL::insert(const std::string& name, int id) {
    bool inserted = false;
    root = insertHelper(root, name, id, inserted);
    if (inserted) {
        nodeCount++;
    }
    return inserted;
}

// ---------- remove ----------

// Standard BST deletion. A node with two children is replaced by its
// inorder successor. No rebalancing is done after deletion.
AVL::Node* AVL::removeHelper(Node* node, int id, bool& removed) {
    if (!node) {
        return nullptr;
    }

    if (id < node->id) {
        node->left = removeHelper(node->left, id, removed);
    }
    else if (id > node->id) {
        node->right = removeHelper(node->right, id, removed);
    }
    else {
        if (!node->left || !node->right) {
            Node* child = node->left ? node->left : node->right;
            delete node;
            removed = true;
            return child;
        }

        Node* successor = node->right;
        while (successor->left) {
            successor = successor->left;
        }
        node->name = successor->name;
        node->id = successor->id;
        node->right = removeHelper(node->right, successor->id, removed);
    }

    updateHeight(node);
    return node;
}

bool AVL::remove(int id) {
    bool removed = false;
    root = removeHelper(root, id, removed);
    if (removed) {
        nodeCount--;
    }
    return removed;
}

bool AVL::removeInorder(int n) {
    if (n < 0 || n >= nodeCount) {
        return false;
    }
    std::vector<Node*> nodes;
    inorderHelper(root, nodes);
    return remove(nodes[n]->id);
}

// ---------- search ----------

AVL::Node* AVL::findID(Node* node, int id) const {
    while (node) {
        if (id == node->id) {
            return node;
        }
        node = id < node->id ? node->left : node->right;
    }
    return nullptr;
}

bool AVL::searchID(int id, std::string& nameOut) const {
    Node* found = findID(root, id);
    if (!found) {
        return false;
    }
    nameOut = found->name;
    return true;
}

// preorder so IDs come out in the order the spec asks for
void AVL::searchNameHelper(Node* node, const std::string& name, std::vector<int>& ids) const {
    if (!node) {
        return;
    }
    if (node->name == name) {
        ids.push_back(node->id);
    }
    searchNameHelper(node->left, name, ids);
    searchNameHelper(node->right, name, ids);
}

std::vector<int> AVL::searchName(const std::string& name) const {
    std::vector<int> ids;
    searchNameHelper(root, name, ids);
    return ids;
}

// ---------- traversals ----------

void AVL::inorderHelper(Node* node, std::vector<Node*>& out) const {
    if (!node) {
        return;
    }
    inorderHelper(node->left, out);
    out.push_back(node);
    inorderHelper(node->right, out);
}

void AVL::preorderHelper(Node* node, std::vector<Node*>& out) const {
    if (!node) {
        return;
    }
    out.push_back(node);
    preorderHelper(node->left, out);
    preorderHelper(node->right, out);
}

void AVL::postorderHelper(Node* node, std::vector<Node*>& out) const {
    if (!node) {
        return;
    }
    postorderHelper(node->left, out);
    postorderHelper(node->right, out);
    out.push_back(node);
}

std::vector<std::string> AVL::inorderNames() const {
    std::vector<Node*> nodes;
    inorderHelper(root, nodes);
    std::vector<std::string> names;
    for (Node* n : nodes) {
        names.push_back(n->name);
    }
    return names;
}

std::vector<std::string> AVL::preorderNames() const {
    std::vector<Node*> nodes;
    preorderHelper(root, nodes);
    std::vector<std::string> names;
    for (Node* n : nodes) {
        names.push_back(n->name);
    }
    return names;
}

std::vector<std::string> AVL::postorderNames() const {
    std::vector<Node*> nodes;
    postorderHelper(root, nodes);
    std::vector<std::string> names;
    for (Node* n : nodes) {
        names.push_back(n->name);
    }
    return names;
}

std::vector<int> AVL::inorderIDs() const {
    std::vector<Node*> nodes;
    inorderHelper(root, nodes);
    std::vector<int> ids;
    for (Node* n : nodes) {
        ids.push_back(n->id);
    }
    return ids; 
} 

std::vector<int> AVL::preorderIDs() const {
    std::vector<Node*> nodes; 
    preorderHelper(root, nodes);
    std::vector<int> ids;
    for (Node* n : nodes) {
        ids.push_back(n->id);
    }
    return ids;
}

std::vector<int> AVL::postorderIDs() const {
    std::vector<Node*> nodes;
    postorderHelper(root, nodes);
    std::vector<int> ids;
    for (Node* n : nodes) {
        ids.push_back(n->id);
    }
    return ids;
}

// ---------- info ----------

int AVL::levelCount() const {
    return height(root);
}

int AVL::size() const {
    return nodeCount;
}

bool AVL::isBalancedHelper(Node* node) const {
    if (!node) {
        return true;
    }
    return std::abs(balanceFactor(node)) <= 1
        && isBalancedHelper(node->left)
        && isBalancedHelper(node->right);
}

bool AVL::isBalanced() const {
    return isBalancedHelper(root);
}
