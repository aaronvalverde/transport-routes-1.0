#ifndef BINARYTREE_H
#define BINARYTREE_H
#include <iostream>
#include "User.h"
#include "Node.h"

using namespace std;

template <typename T>
class UserTree {
    public:
    int getMax(int leftHeight, int rightHeight);
    void updateHeight(Node<User>*& root);
    void updateBF(Node<User>*& root);
    void LL(Node<User>*& root);
    void RR(Node<User>*& root);
    void LR(Node<User>*& root);
    void RL(Node<User>*& root);
    void add(Node<User>*& root, User* user);
    Node<T> find(Node<User>* root, User* user);
    void remove(Node<User>*& root, User* user);
    void preOrder(Node<User>* root);
    void inOrder(Node<User>* root);
    void postOrder(Node<User>* root);
};


#endif