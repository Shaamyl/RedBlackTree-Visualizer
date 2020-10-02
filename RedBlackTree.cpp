#include "RedBlackTree.h"
#include <iostream>
#include <stdexcept>
#include <cassert>
//note: added appropriate tests to RedBlackTreeTests.cpp

string RedBlackTree::CreateDotString() const{
    string finalS = "digraph rbt{\n";
    finalS += CreateDotString(root);
    finalS += "}\n";
    return finalS;
}

string RedBlackTree::CreateDotString(RBTNode* root) const{ //function to help visualize tree
    if(root == nullptr){
        return "";
    }
    string currentNodeS = "";

    if(root->leftChild != nullptr){ //order important to maintain bst property in dot (left node comes first)
    currentNodeS += to_string(root->data) + string("->") + to_string(root->leftChild->data) + string(";\n");
    }
    if(root->rightChild != nullptr){
    currentNodeS += to_string(root->data) + string("->") + to_string(root->rightChild->data) + string(";\n");
    }
    if(root->color == REDBLACKTREE_COLOR_BLACK){
        currentNodeS += to_string(root->data) + string("[style = \"filled\" fillcolor = \"black\" fontcolor=\"white\"];\n");
    }else{
        currentNodeS += to_string(root->data) + string("[style = \"filled\" fillcolor = \"red\" fontcolor=\"white\"];\n");
    }
    currentNodeS += CreateDotString(root->leftChild);
    currentNodeS += CreateDotString(root->rightChild);
    return currentNodeS;
}


bool RedBlackTree::Contains(int data) const{

    return ContainsHelper(data, this->root);
}

bool RedBlackTree::ContainsHelper(int data, RBTNode* current) const{ //recursive private function to keep public interface encapsulated, goes left if data is smaller and right if data is bigger
    if(current == nullptr){
        return false;
    }else if(data == current->data){
        return true;
    }else if(data > current->data){
        return ContainsHelper(data, current->rightChild);
    }else if(data < current->data){
        return ContainsHelper(data, current->leftChild);
    }

    throw "Invalid BST in Contains()"; //code should never reach here, invalid BST
    return false;
}

int RedBlackTree::GetMin() const{
    if(root == nullptr){ //edge case for empty tree
        throw length_error("Invalid GetMin() call on empty tree");
    }

    RBTNode* current = root;
    while(current->leftChild != nullptr){
        current = current->leftChild;
    }

    return current->data;
}

int RedBlackTree::GetMax() const{
    if(root == nullptr){ //edge case for empty tree
        throw length_error("Invalid GetMax() call on empty tree");
    }

    RBTNode* current = root;
    while(current->rightChild != nullptr){
        current = current->rightChild;
    }

    return current->data;
}

string RedBlackTree::ToInfixString (RBTNode* root) const{
    if(root == nullptr){
        return "";
    }else{
        string currentNodeS = string(" ") + string(root->color == REDBLACKTREE_COLOR_RED ? "R" : "B") + to_string(root->data) + string(" "); //create current node's string repr.
        return ToInfixString(root->leftChild) + currentNodeS + ToInfixString(root->rightChild) ; //infix order
    }
}

string RedBlackTree::ToPrefixString (RBTNode* root) const{
    if(root == nullptr){
        return "";
    }else{
        string currentNodeS = string(" ") + string(root->color == REDBLACKTREE_COLOR_RED ? "R" : "B") + to_string(root->data) + string(" "); //create current node's string repr.
        return currentNodeS + ToPrefixString(root->leftChild) + ToPrefixString(root->rightChild); //prefix order
    }
}

string RedBlackTree::ToPostfixString (RBTNode* root) const{
    if(root == nullptr){
        return "";
    }else{
        string currentNodeS = string(" ") + string(root->color == REDBLACKTREE_COLOR_RED ? "R" : "B") + to_string(root->data) + string(" "); //create current node's string repr.
        return ToPostfixString(root->leftChild) + ToPostfixString(root->rightChild) + currentNodeS; //postfix order
    }
}

void RedBlackTree::Insert(int data){
    RBTNode* newNode = new RBTNode(data); //create new node from int

    //normal BST insertion
    bool parentFound = false;
    if(root == nullptr){ //case where tree is empty
        root = newNode;
        parentFound = true;
    }

    RBTNode* currentNode = root;
    while(!parentFound){
        if(data == currentNode-> data){
            throw invalid_argument( "Duplicate item " + to_string(data) + " passed to Insert()" ); //if data is duplicate, throw except.
        }else if(data > currentNode->data && currentNode->rightChild != nullptr){
            currentNode = currentNode->rightChild;
        }else if(data < currentNode->data  && currentNode->leftChild != nullptr){
            currentNode = currentNode->leftChild;
        }else if(data > currentNode->data){
            currentNode->rightChild = newNode;
            newNode->parent = currentNode;
            numItems++;
            parentFound = true;
        }else if(data < currentNode->data){
            currentNode->leftChild = newNode;
            newNode->parent = currentNode;
            numItems++;
            parentFound = true;
        }
    }

    //starting fix property violations algorithm
    RBTNode* cNode = newNode;
    while(cNode != root && cNode->parent->color != REDBLACKTREE_COLOR_BLACK){
        if(GetUncle(cNode) != nullptr && GetUncle(cNode)->color == REDBLACKTREE_COLOR_RED){ //note: will not be called with invalid args because root is always black
            //case if uncle is red
            //recolor
            cNode->parent->color = REDBLACKTREE_COLOR_BLACK;
            GetUncle(cNode)->color = REDBLACKTREE_COLOR_BLACK;
            cNode->parent->parent->color = REDBLACKTREE_COLOR_RED;
            cNode = cNode->parent->parent; //make cNode as grandparent
        }else if(IsRightChild(cNode)){ //cnode is right child, two cases for parent
            if(IsRightChild(cNode->parent)){ //parent is right child
                RotateLeft(cNode->parent->parent);
                int temp = cNode->parent->color;
                cNode->parent->color = cNode->parent->leftChild->color;
                cNode->parent->leftChild->color = temp;
                return;
            }else{ //parent is left child
                RotateLeft(cNode->parent);
                cNode = cNode->leftChild;
            }
        }else{ //cNode is left child
            if(IsRightChild(cNode->parent)){ //parent is right child
                RotateRight(cNode->parent);
                cNode = cNode->rightChild;
            }else{ //parent is left child
                RotateRight(cNode->parent->parent);
                int temp = cNode->parent->color;
                cNode->parent->color = cNode->parent->rightChild->color;
                cNode->parent->rightChild->color = temp;
                return;
            }
        }
    }
    if(cNode == root){ //first case when cNode is root
        cNode->color = REDBLACKTREE_COLOR_BLACK;
        return;
    }
}

void RedBlackTree::RotateLeft(RBTNode* root){
    if(root->rightChild == nullptr){
        return;
    }
    RBTNode* rightNode = root->rightChild;
    if(rightNode->leftChild != nullptr){ //do first part of rotate left algorithm
        root->rightChild = rightNode->leftChild;
        rightNode->leftChild->parent = root;
    }else{
        root->rightChild = nullptr;
    }

    if(root->parent != nullptr){
        if(root->parent->leftChild == root){ //if root is left child of its parent
            root->parent->leftChild = rightNode;
        }else if(root->parent->rightChild == root){
            root->parent->rightChild = rightNode;
        }
        rightNode->parent = root->parent;

    }else{
        this->root = rightNode;
        rightNode->parent = nullptr;
    }

    rightNode->leftChild = root;
    root->parent = rightNode;
}

void RedBlackTree::RotateRight(RBTNode* root){
    if(root->leftChild == nullptr){
        return;
    }
    RBTNode* leftNode = root->leftChild;
    if(leftNode->rightChild != nullptr){ //do first part of rotate left algorithm
        root->leftChild = leftNode->rightChild;
        leftNode->rightChild->parent = root;
    }else{
        root->leftChild = nullptr;
    }

    if(root->parent != nullptr){
        if(root->parent->leftChild == root){ //if root is left child of its parent
            root->parent->leftChild = leftNode;
        }else if(root->parent->rightChild == root){
            root->parent->rightChild = leftNode;
        }
        leftNode->parent = root->parent;

    }else{
        this->root = leftNode;
        leftNode->parent = nullptr;
    }

    leftNode->rightChild = root;
    root->parent = leftNode;
}

bool RedBlackTree::IsRightChild(RBTNode* root) const{
    if(root->parent == nullptr){
        throw invalid_argument("Invalid node to IsRightChild(), no parent"); //no parent case
    }
    if(root->parent->rightChild == root){
        return true;
    }else{
        return false;
    }
}

RBTNode* RedBlackTree::GetUncle(RBTNode* root) const{
    if(root->parent == nullptr || root->parent->parent == nullptr){
        throw invalid_argument("Invalid node to GetUncle(), either no parent or grandparent");
    }
    if(IsRightChild(root->parent)){
        return root->parent->parent->leftChild;
    }else{
        return root->parent->parent->rightChild;
    }
}

RedBlackTree::RedBlackTree(const RedBlackTree& otherTree){
    CopyHelper(otherTree, otherTree.root, nullptr);
}

void RedBlackTree::CopyHelper(const RedBlackTree& otherTree, RBTNode* current, RBTNode* parent){
    if(current == nullptr){
        return;
    }//base case
    RBTNode* newNode = new RBTNode(current->data);
    newNode->color = current->color;
    if(parent == nullptr){ //if no parent, i.e root
        this->root = newNode;
    }else{ //if not root, has parent
        if(current->parent->leftChild == current){ //check if original tree's current was left or right.
            parent->leftChild = newNode;           //we've passed new tree's parent node so that we can attach newNode as right or left to parent
            newNode->parent = parent;
        }else if(current->parent->rightChild == current){
            parent->rightChild = newNode;
            newNode->parent = parent;
        }
    }
    CopyHelper(otherTree, current->leftChild, newNode);
    CopyHelper(otherTree, current->rightChild, newNode);
}

RedBlackTree::~RedBlackTree(){//implement to properly deallocate memory
    DestructorHelper(root);
}

void RedBlackTree::DestructorHelper(RBTNode* root){
    if(root == nullptr){
        return;
    }

    DestructorHelper(root->leftChild); //recurse both children, do a level order type traversal, destroy every node
    DestructorHelper(root->rightChild);
    delete root;
}

/* main for testing int main(){
    RBTNode* root = new RBTNode(2);

    RedBlackTree simpleRBT(root);

    simpleRBT.Insert(1);
    simpleRBT.Insert(3);

    cout << "Testing" << endl;
    assert(simpleRBT.Contains(5) == 0);
    assert(simpleRBT.Contains(1) == 1);
    assert(simpleRBT.Contains(2) == 1);
    assert(simpleRBT.Contains(3) == 1);
    assert(simpleRBT.Contains(-1) == 0);

    cout << "Contains() tests passed!"<< endl;

    assert(simpleRBT.GetMax() == 3);
    assert(simpleRBT.GetMin() == 1);

    cout << "GetMax() and GetMin() tests passed!"<< endl;

    assert(simpleRBT.Size() == 3);

    cout << "Size() tests passed!"<< endl;

    cout << "Infix: " << simpleRBT.ToInfixString() << endl;
    cout << "Prefix: " << simpleRBT.ToPrefixString() << endl;
    cout << "Postfix: " << simpleRBT.ToPostfixString() << endl;


    RedBlackTree rBT2;

    rBT2.Insert(30);
    rBT2.Insert(15);
    rBT2.Insert(10);

    cout<< "2nd Tree Infix: " << rBT2.ToInfixString() << endl;
    //rBT2.TestPrivate();

    return 0;
}*/
