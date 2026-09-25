#pragma once

#include <string>
#include <vector>

struct Node {
        std::string name;
        int id;
        int height;
        Node* left;
        Node* right;

        Node(const std::string& name, int id){
            this->name = name;
            this->id = id;
            this->left = nullptr;
            this->right = nullptr;
        }
}

class AVL {
private:
    
    Node* root = nullptr;
    Node* insertHelper(Node* node, std::string name, std::string id);

    public:
    void insert(std::string name, std::string id);

    ~AVL(){
        // WRITE POSTORDER TRAVERSAL TO DELETE ALL NODES
    };

    Node* root;
    int nodeCount;

    // height / balance helpers
    int getHeight(Node* node) const;
    void updateHeight(Node* node);
    int balanceFactor(Node* node) const;

    // rotations
    Node* rotateLeft(Node* node);
    Node* rotateRight(Node* node);
    Node* rotateLeftRight(Node* node);
    Node* rotateRightLeft(Node* node);

    // recursive helpers
    Node* insertHelper(Node* node, const std::string& name, int id){};
    Node* removeHelper(Node* node, int id);
    Node* findID(Node* node, int id) const;
    void searchNameHelper(Node* node, const std::string& name, std::vector<int>& ids) const; // did not do it; necessary?
    void inorderHelper(Node* node, std::vector<Node*>& out) const; 
    void preorderHelper(Node* node, std::vector<Node*>& out) const; 
    void postorderHelper(Node* node, std::vector<Node*>& out) const; 
    bool isBalancedHelper(Node* node) const;
    void destroy(Node* node);

public:
    AVL();
    ~AVL();
    AVL(const AVL&) = delete;
    AVL& operator=(const AVL&) = delete;

    bool insert(const std::string& name, int id);
    bool remove(int id);
    bool removeInorder(int n);

    bool searchID(int id, std::string& nameOut) const;
    std::vector<int> searchName(const std::string& name) const;

    std::vector<std::string> inorderNames() const;
    std::vector<std::string> preorderNames() const;
    std::vector<std::string> postorderNames() const;

    // ID versions of the traversals, mainly for testing
    std::vector<int> inorderIDs() const;
    std::vector<int> preorderIDs() const;
    std::vector<int> postorderIDs() const;

    int levelCount() const;
    int size() const;
    bool isBalanced() const;
};
