#ifndef USERLOGIN_H
#define USERLOGIN_H

#include <iostream>

using namespace std;

class user
{
private:
    unsigned long long int id;
    string password;

public:
    user(unsigned long long int _id, string _password)
        : id(_id), password(_password) {}

    unsigned long long int get_id() {return id;}
    string get_password() {return password;}
};

#endif // USERLOGIN_H
