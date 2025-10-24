#include "Node.h"

template <typename T>
Node<T>::Node(T data){
    this->data = data;
    left = nullptr;
    right = nullptr;
}