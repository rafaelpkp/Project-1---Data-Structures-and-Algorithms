#include "AVL.h"

#include <algorithm>
#include <cstdlib>

// Basic functions
int getHeight(Node* node){
    int height;

    int left_height = (node->left != nullptr ? node->left->height : 0);
    int right_height = (node->right != nullptr ? node->right->height : 0);

    height = 1 + std::max(left_height, right_height);

    return height;
};

// Rotations
Node* rotateLeft(Node* node){
    Node* root = node->right;
    Node* temp = root->left;

    root->left = node;
    node->right = temp;

    return root;
};

Node* rotateRight(Node* node){
    Node* root = node->left;
    Node* temp = root->right;

    root->right = node;
    node->left = temp;

    return root;
};

Node* rotateLeftRight(Node* node){
    Node* root = node->left->right;
    Node* temp = root->left;

    root->left = node->left;
    root->right = node;
};

Node* rotateRightLeft(Node* node){
    Node* root = node->right->left;
    Node* temp = root->right;

    root->left = node;
    root->right = node->right;
};



// Helper functions
Node* insertHelper(Node* node, const std::string& name, int id){
    // Add node
    if(node ==  nullptr){
        return new Node(name, id);
    }

    else if(id < node->id){
        node->left = insertHelper(node, name, id);
    }

    else{
        node->left = insertHelper(node, name, id);
    } 

    // Update height
    node->height = getHeight(node);

    // Perform rotations
    if(node->right->height > node->left->height){ // if tree is right heavy
        if(node->right->right->height < node->right->left->height){ // if tree's right subtree is left heavy
            rotateRightLeft(node);
            node->height = getHeight(node);
        }

        else{
            rotateLeft(node);
            node->height = getHeight(node);
        }
    }

    if(node->right->height < node->left->height){ // if tree is leaft heavy
        if(node->left->left->height < node->left->right->height){ // if tree's left subtree is right heavy
            rotateLeftRight(node);
            node->height = getHeight(node);
        }

        else{
            rotateRight(node);
            node->height = getHeight(node);
        }
    }
};

Node* removeHelper(Node* node, int id){

    if(node == nullptr){
        return;
    }

    else if(id < node->id){
        node->left = removeHelper(node->left, id);
    }

    else if(id > node->id){
        node->right = removeHelper(node->right, id);
    }

    else{ // id == node->id
        if(node->left == nullptr && node->right == nullptr){ // no childrens
            delete node;
            return nullptr;
        }

        else if (node->left != nullptr && node->right == nullptr){ // has a left children
            // set the parent to the left child
            Node* temp = node->left;
            delete node;
            return temp;
        }

        else if (node->right != nullptr && node->left == nullptr){ // has a right children
            // set the parent to the right child
            Node* temp = node->right;
            delete node;
            return temp;
        }

        else{ // has both childrens
            if(node->left->right == nullptr){
                // set the parent to the left child
                Node* temp = node->left;
                delete node;
                return temp;
            }

            else{
                Node* curr = node;

                while(curr->right != nullptr){
                    curr = curr->right;
                }

                // set the parent to curr
                Node* temp = curr;
                delete curr;
                return temp;
            }
        }
        
    }

    return node;
};

Node* findID(Node* node, int id){
    if(node == nullptr){
        return nullptr;
    }

    else if(id < node->id){
        return findID(node->left, id);
        }

    else if(id > node->id){
        return findID(node->right, id);
    }

    else{
        return node;
    }
};

void searchNameHelper(Node* node, const std::string& name, std::vector<int>& ids){
    if(node->)

};

// main functions
void inorderHelper(Node* node){
     
    if(node == nullptr){
        return "";
    }

    inorderHelper(node->left);
    std::cout << node->name << node->id << std::endl;
    inorderHelper(node->right);
};

void preorderHelper(Node* node){
     
    if(node == nullptr){
        return "";
    }

    std::cout << node->name << node->id << std::endl;  
    preorderHelper(node->left);
    preorderHelper(node->right);  
};

void postorderHelper(Node* node){
     
    if(node == nullptr){
        return "";
    }

    postorderHelper(node->left);
    postorderHelper(node->right);
    std::cout << node->name << node->id << std::endl;  
};






// void AVL::insert(std::string name, std::string id){
//     this->root = insertHelper(this->root, name, id);
// }

// void delete;




