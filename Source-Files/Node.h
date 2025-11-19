#ifndef NODE_H
#define NODE_H

#include <iostream>

using namespace std;

template <typename T>
class Node {
public:
    T data;
    Node<T>* left;
    Node<T>* right;
    int height;
    int bf;
    
    Node();
    Node(T data);
};

template <typename T>
Node<T>::Node(){
    data = T();
    left = nullptr;
    right = nullptr;
    height = 0;
    bf = 0;
}

template <typename T>
Node<T>::Node(T data){
    this->data = data;
    left = nullptr;
    right = nullptr;
    height = 0;
    bf = 0;
}

#endif