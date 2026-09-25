#include "AVL.h"

#include <algorithm>
#include <cstdlib>

AVL::AVL(){
    root = nullptr;
    nodeCount = 0;
}

AVL::~AVL(){
    destroy(root);
}

// Postorder traversal so children are freed before their parent
void AVL::destroy(Node* node){
    if(node == nullptr){ 
        return;
    }

    destroy(node->left);
    destroy(node->right);
    delete node;
}

// Height / balance helpers
int AVL::getHeight(Node* node) const{
    return (node != nullptr ? node->height : 0);
}

void AVL::updateHeight(Node* node){
    node->height = 1 + std::max(getHeight(node->left), getHeight(node->right));
}

// Positive means left heavy, negative means right heavy
int AVL::balanceFactor(Node* node) const{
    return getHeight(node->left) - getHeight(node->right);
}

// Picks one of the four rotations if the node is out of balance
Node* AVL::rebalance(Node* node){
    updateHeight(node);
    int balance = balanceFactor(node);

    if(balance > 1){ // left heavy
        if(balanceFactor(node->left) < 0){ // left subtree is right heavy
            return rotateLeftRight(node);
        }
        return rotateRight(node);
    }

    if(balance < -1){ // right heavy
        if(balanceFactor(node->right) > 0){ // right subtree is left heavy
            return rotateRightLeft(node);
        }
        return rotateLeft(node);
    }

    return node;
}

// Rotations
Node* AVL::rotateLeft(Node* node){
    Node* newRoot = node->right;
    Node* temp = newRoot->left;

    newRoot->left = node;
    node->right = temp;

    updateHeight(node);
    updateHeight(newRoot);
    return newRoot;
}

Node* AVL::rotateRight(Node* node){
    Node* newRoot = node->left;
    Node* temp = newRoot->right;

    newRoot->right = node;
    node->left = temp;

    updateHeight(node);
    updateHeight(newRoot);
    return newRoot;
}

Node* AVL::rotateLeftRight(Node* node){
    node->left = rotateLeft(node->left);
    return rotateRight(node);
}

Node* AVL::rotateRightLeft(Node* node){
    node->right = rotateRight(node->right);
    return rotateLeft(node);
}

// Insert
Node* AVL::insertHelper(Node* node, const std::string& name, int id, bool& inserted){
    if(node == nullptr){
        inserted = true;
        return new Node(name, id);
    }

    if(id < node->id){
        node->left = insertHelper(node->left, name, id, inserted);
    }
    else if(id > node->id){
        node->right = insertHelper(node->right, name, id, inserted);
    }
    else{ // duplicate ID
        return node;
    }

    return rebalance(node);
}

bool AVL::insert(const std::string& name, int id){
    bool inserted = false;
    root = insertHelper(root, name, id, inserted);
    if(inserted){
        nodeCount++;
    }
    return inserted;
}

// Remove
Node* AVL::removeHelper(Node* node, int id, bool& removed){
    if(node == nullptr){
        return nullptr;
    }

    if(id < node->id){
        node->left = removeHelper(node->left, id, removed);
    }
    else if(id > node->id){
        node->right = removeHelper(node->right, id, removed);
    }
    else{ // id == node->id
        if(node->left == nullptr || node->right == nullptr){ // zero or one child
            Node* child = (node->left != nullptr ? node->left : node->right);
            delete node;
            removed = true;
            return child;
        }

        // two children: copy the inorder successor, then remove it from the right subtree
        Node* successor = node->right;
        while(successor->left != nullptr){
            successor = successor->left;
        }
        node->name = successor->name;
        node->id = successor->id;
        node->right = removeHelper(node->right, successor->id, removed);
    }

    return rebalance(node);
}

bool AVL::remove(int id){
    bool removed = false;
    root = removeHelper(root, id, removed);
    if(removed){
        nodeCount--;
    }
    return removed;
}

bool AVL::removeInorder(int n){
    if(n < 0 || n >= nodeCount){
        return false;
    }

    std::vector<Node*> nodes;
    inorderHelper(root, nodes);
    return remove(nodes[n]->id);
}

// Search
Node* AVL::findID(Node* node, int id) const{
    while(node != nullptr && node->id != id){
        node = (id < node->id ? node->left : node->right);
    }
    return node;
}

bool AVL::searchID(int id, std::string& nameOut) const{
    Node* found = findID(root, id);
    if(found == nullptr){
        return false;
    }
    nameOut = found->name;
    return true;
}

// Preorder so matching IDs come out in preorder
void AVL::searchNameHelper(Node* node, const std::string& name, std::vector<int>& ids) const{
    if(node == nullptr){
        return;
    }

    if(node->name == name){
        ids.push_back(node->id);
    }
    searchNameHelper(node->left, name, ids);
    searchNameHelper(node->right, name, ids);
}

std::vector<int> AVL::searchName(const std::string& name) const{
    std::vector<int> ids;
    searchNameHelper(root, name, ids);
    return ids;
}

// Traversals
void AVL::inorderHelper(Node* node, std::vector<Node*>& out) const{
    if(node == nullptr){
        return;
    }

    inorderHelper(node->left, out);
    out.push_back(node);
    inorderHelper(node->right, out);
}

void AVL::preorderHelper(Node* node, std::vector<Node*>& out) const{
    if(node == nullptr){
        return;
    }

    out.push_back(node);
    preorderHelper(node->left, out);
    preorderHelper(node->right, out);
}

void AVL::postorderHelper(Node* node, std::vector<Node*>& out) const{
    if(node == nullptr){
        return;
    }

    postorderHelper(node->left, out);
    postorderHelper(node->right, out);
    out.push_back(node);
}

std::vector<std::string> AVL::namesOf(const std::vector<Node*>& nodes){
    std::vector<std::string> names;
    for(Node* node : nodes){
        names.push_back(node->name);
    }
    return names;
}

std::vector<int> AVL::idsOf(const std::vector<Node*>& nodes){
    std::vector<int> ids;
    for(Node* node : nodes){
        ids.push_back(node->id);
    }
    return ids;
}

std::vector<std::string> AVL::inorderNames() const{
    std::vector<Node*> nodes;
    inorderHelper(root, nodes);
    return namesOf(nodes);
}

std::vector<std::string> AVL::preorderNames() const{
    std::vector<Node*> nodes;
    preorderHelper(root, nodes);
    return namesOf(nodes);
}

std::vector<std::string> AVL::postorderNames() const{
    std::vector<Node*> nodes;
    postorderHelper(root, nodes);
    return namesOf(nodes);
}

std::vector<int> AVL::inorderIDs() const{
    std::vector<Node*> nodes;
    inorderHelper(root, nodes);
    return idsOf(nodes);
}

std::vector<int> AVL::preorderIDs() const{
    std::vector<Node*> nodes;
    preorderHelper(root, nodes);
    return idsOf(nodes);
}

std::vector<int> AVL::postorderIDs() const{
    std::vector<Node*> nodes;
    postorderHelper(root, nodes);
    return idsOf(nodes);
}

// Other info
int AVL::levelCount() const{
    return getHeight(root);
}

int AVL::size() const{
    return nodeCount;
}

bool AVL::isBalancedHelper(Node* node) const{
    if(node == nullptr){
        return true;
    }

    int balance = balanceFactor(node);
    if(balance > 1 || balance < -1){
        return false;
    }
    return isBalancedHelper(node->left) && isBalancedHelper(node->right);
}

bool AVL::isBalanced() const{
    return isBalancedHelper(root);
}
