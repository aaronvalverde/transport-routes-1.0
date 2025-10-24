#ifndef NODE_H
#define NODE_H
#include <iostream>

using namespace std;

template <typename T>
class Node {
    private: 
    T data;
    Node* left;
    Node* right;

    public:
    Node(T data);
};

#endif