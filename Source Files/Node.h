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
    int height;
    int bf;

    public:
    Node();
    Node(T data);
};

#endif