#ifndef REDBLACKTREE_H
#define REDBLACKTREE_H
#define REDBLACKTREE_COLOR_RED 0
#define REDBLACKTREE_COLOR_BLACK 1
#include <string>
#include <iostream>

using namespace std;

struct RBTNode{
    int data;
    int color = REDBLACKTREE_COLOR_RED; //by default, red
    RBTNode* parent;
    RBTNode* leftChild;
    RBTNode* rightChild;

    RBTNode(int data = 0, RBTNode* parent = nullptr, RBTNode* leftChild = nullptr, RBTNode* rightChild = nullptr){
        this->data = data;
        this-> parent = parent;
        this->leftChild = leftChild;
        this->rightChild = rightChild;
    }
};

class RedBlackTree{
public:
    RedBlackTree(RBTNode* root){ root->color = REDBLACKTREE_COLOR_BLACK; this->root = root; numItems = 1;};
    RedBlackTree(){};
    RedBlackTree(const RedBlackTree& otherTree);
    ~RedBlackTree(); //implement so that memory deallocated properly
    bool Contains(int data) const;
    int GetMin() const;
    int GetMax() const;
    unsigned long long Size() const{ return numItems; };
    void Insert(int data);
    string ToInfixString() const { return ToInfixString(root);};
    string ToPrefixString() const { return ToPrefixString(root);};
    string ToPostfixString() const { return ToPostfixString(root);};
    string CreateDotString() const;
    void TestPrivate(){
        this->RotateLeft(root);
        cout << "Tree Infix After Rotate Left " << this->ToInfixString() << endl;
        this->RotateRight(root);
        cout << "Tree Infix After Rotate Right " << this->ToInfixString() << endl;
    }

private:
    RBTNode* root = nullptr;
    unsigned long long int numItems = 0;
    string ToInfixString (RBTNode* root) const;
    string ToPrefixString(RBTNode* root) const;
    string ToPostfixString(RBTNode* root) const;
    void RotateLeft(RBTNode* root);
    void RotateRight(RBTNode* root);
    bool IsRightChild(RBTNode* root) const;
    RBTNode* GetUncle(RBTNode* root) const;
    bool ContainsHelper(int data, RBTNode* current) const;
    void CopyHelper(const RedBlackTree& otherTree, RBTNode* current, RBTNode* parent);
    void DestructorHelper(RBTNode* root);
    string CreateDotString(RBTNode* root) const;
};
#endif
