#ifndef LOGIN_H
#define LOGIN_H

#include <iostream>

using namespace std;

bool login_admin(unsigned int seed);

bool add_user(unsigned long long int id, string password, unsigned seed);

bool search_id(string text, unsigned long long int len, unsigned long long int id, unsigned long long int &index, unsigned long long int &end_index);

bool valid_password(string password);

bool register_user(unsigned long long int id, unsigned seed);

bool login(unsigned int seed, bool &is_admin);

#endif // LOGIN_H
