#include "Node.h"

template <typename T>
Node<T>::Node(){
    data = null;
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