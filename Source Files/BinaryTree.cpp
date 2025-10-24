#include "BinaryTree.h"

template <typename T>
void BinaryTree<T>::add(Node<T>*& root, T value){
    if(!root){
        root = new Node(value);
        return;
    }
    if (root->data > value){
        add(root->left, value);
        return;
    }
    add(root->right, value);
}

template <typename T>
Node<T> BinaryTree<T>::find(Node<T>* root, T value){
    if(!root){
        return;
    }
    if (root->data == value){
        return root;
        return;
    }
    if(root->data > value){
        find(root->left, value);
        return;
    }
    find(root->right, value);
}

template <typename T>
void BinaryTree<T>::remove(Node<T>*& root, T value){
    if(!root){
        return;
    }
    //TODO
}

template <typename T>
void BinaryTree<T>::preOrder(Node<T>* root){
    cout << root->data;
    preOrder(root->left);
    preOrder(root->right);
}

template <typename T>
void BinaryTree<T>::inOrder(Node<T>* root){
    inOrder(root->left);
    cout << root->data;
    inOrder(root->right);
}

template <typename T>
void BinaryTree<T>::postOrder(Node<T>* root){
    postOrder(root->left);
    postOrder(root->right);
    cout << root;
}