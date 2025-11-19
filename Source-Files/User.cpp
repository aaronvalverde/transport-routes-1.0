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

long User::getID(){
    return ID;
}

string User::getName() const{
    return name;
}

int User::getAge(){
    return age;
}

long User::getBalance(){
    return balance;
}

void User::setBalance(long balance){
    this->balance = balance;
}