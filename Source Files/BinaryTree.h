#ifndef BINARYTREE_H
#define BINARYTREE_H
#include <iostream>
#include "Node.h"

using namespace std;

template <typename T>
class BinaryTree {
    public:
    void add(Node<T>*& root, T value);
    Node<T> find(Node<T>* root, T value);
    void remove(Node<T>*& root, T value);
    void preOrder(Node<T>* root);
    void inOrder(Node<T>* root);
    void postOrder(Node<T>* root);
};


#endif