#include "User.h"

User::User(){
    ID = 0;
    name = " ";
    age = 0;
    balance = 0;
}

User::User(long ID, string name, int age, long balance){
    this->ID = ID;
    this->name = name; 
    this->age = age;
    this->balance = balance;
}

void User::print(){
    cout << "ID: " << ID << endl;
    cout << "Nombre: " << name << endl;
    cout << "Edad: " << age << endl;
    cout << "Saldo: " << balance << endl;
}

string User::getName(){
    return name;
}