#ifndef LOGIN_H
#define LOGIN_H

#include <iostream>

using namespace std;

class User
{
private:
    unsigned long long int id;
    string password;

public:
    User(unsigned long long int _id, string _password)
        : id(_id), password(_password) {}

    unsigned long long int get_id() {return id;}
    string get_password() {return password;}
};

#endif // LOGIN_H
