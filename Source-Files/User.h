#ifndef USER_H
#define USER_H
#include <iostream>

using namespace std;

class User {
    private:
    long ID;
    string name;
    int age;
    long balance;

    public:
    User();
    User(long ID, string name, int age, long balance);
    void print();

    long getID() const;
    string getName() const;
    int getAge() const;
    long getBalance() const;
    void setBalance(long balance);
};


#endif