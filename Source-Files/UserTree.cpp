#include "UserTree.h"

template <typename T>
int UserTree<T>::getMax(int leftHeight, int rightHeight){
    if(leftHeight > rightHeight){
        return leftHeight;
    }
    return rightHeight;
}

template <typename T>
void UserTree<T>::updateHeight(Node<User>*& root){
   int leftHeight = 0, rightHeight = 0;  
   if(root->left == nullptr){
    leftHeight = 0;
   } else {
    updateHeight(root->left);
    leftHeight = root->left->height;
   }

   if(root->right == nullptr){
    rightHeight = 0;  
   } else {
    updateHeight(root->right);
    rightHeight = root->right->height;  
   }

   root->height = getMax(leftHeight, rightHeight) + 1;
}

template <typename T>
void UserTree<T>::updateBF(Node<User>*& root){
   int leftHeight, rightHeight = 0; 
   if(root->left == nullptr){
    leftHeight = 0;
   } else {
    updateBF(root->left);
    leftHeight = root->left->height;
   }

   if(root->right == nullptr){
    rightHeight = 0; 
   } else {
    updateBF(root->right);
    rightHeight = root->right->height; 
   }

   root->bf = rightHeight - leftHeight;
}

template <typename T>
void UserTree<T>::LL(Node<User>*& root){
    Node<User>* aux = root->left;
    root->left = aux->right;
    aux->right = root;
    root = aux;
 
    updateHeight(root->right);
    updateHeight(root);
    updateBF(root->right);
    updateBF(root);
}

template <typename T>
void UserTree<T>::RR(Node<User>*& root){
    Node<User>* aux = root->right;
    root->right = aux->left;
    aux->left = root;
    root = aux;

    updateHeight(root->left);
    updateHeight(root);
    updateBF(root->left);
    updateBF(root);
}

template <typename T>
void UserTree<T>::LR(Node<User>*& root){
    RR(root->left);
    LL(root);
}

template <typename T>
void UserTree<T>::RL(Node<User>*& root){
    LL(root->right);
    RR(root);
}

template <typename T>
void UserTree<T>::add(Node<User>*& root, User* user){
    if(!root){
        root = new Node<User>(*user);
    }
    else if (root->data.getID() > user->ID){
       add(root->left, user);
    } else {
       add(root->right, user);
    }

    updateHeight(root);
    updateBF(root);

    if(root->bf < -1){
        if (root->left->bf < 0){
            LL(root);
        }
        else if(root->left->bf > 0){
            LR(root);
        }
    }
    else if (root->bf > 1){
        if(root->right->bf > 0){
            RR(root);
        }
        else if(root->right->bf < 0){
            RL(root);
        }
    }
}

template <typename T>
Node<User>* UserTree<T>::find(Node<User>* root, User* user){ 
    if(!root){
        return nullptr;  
    }
    if (root->data.getID() == user->ID){
        return root; 
    }
    if(root->data.getID() > user->ID){
        return find(root->left, user);  
    }
    return find(root->right, user);
}

template <typename T>
void UserTree<T>::remove(Node<User>*& root, User* value){
    if(!root){
        return;
    }
    if(value->ID < root->data.getID()){
        remove(root->left, value);
    }
    else if(value->ID > root->data.getID()){
        remove(root->right, value);
    }
    else {
        if(!root->left || !root->right){
            Node<User>* temp = root->left ? root->left : root->right;
            if(!temp){
                delete root;
                root = nullptr;
            } else {
                Node<User>* toRemove = root;
                root = temp;
                delete toRemove;
            }
        } else {
            Node<User>* temp = root->right;
            while(temp->left){
                temp = temp->left;
            }
            root->data = temp->data;
            User tempUser;
            tempUser.ID = temp->data.getID();
            remove(root->right, &tempUser);
        }
    }

    if(!root) {
        return;
    }

    updateHeight(root);
    updateBF(root);

    if (root->bf < -1){
        if(root->left && root->left->bf <= 0){
            LL(root);
        } 
        else if (root->left){
            LR(root);
        }
    }
    else if(root->bf > 1){
        if(root->right && root->right->bf >= 0){
            RR(root);
        }
        else if (root->right){
            RL(root);
        }
    }
}

template <typename T>
void UserTree<T>::preOrder(Node<User>* root){
    if(!root) return;
    root->data.print();
    preOrder(root->left);
    preOrder(root->right);
}

template <typename T>
void UserTree<T>::inOrder(Node<User>* root){
    if(!root) return;
    inOrder(root->left);
    root->data.print();
    inOrder(root->right);
}

template <typename T>
void UserTree<T>::postOrder(Node<User>* root){
    if(!root) return;
    postOrder(root->left);
    postOrder(root->right);
    root->data.print();
}

template <typename T>
void UserTree<T>::printAlphabetically(Node<User>* root){
    vector<User> users;
    addToListHelper(root, users);
    
    sort(users.begin(), users.end(),
         [](const User& a, const User& b){
             return a.getName() < b.getName();
         });
    
    for(auto& user : users){
        user.print();
    }
}

template <typename T>
void UserTree<T>::addToListHelper(Node<User>* root, vector<User>& users){
    if(!root) return;
    
    users.push_back(root->data);
    addToListHelper(root->left, users);
    addToListHelper(root->right, users);
}