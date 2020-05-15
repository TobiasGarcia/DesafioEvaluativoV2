#ifndef LOGIN_H
#define LOGIN_H

#include <iostream>

using namespace std;

bool login_admin(unsigned int seed);

bool search_id(string text, unsigned long long int len, unsigned long long int id, unsigned long long int &index, unsigned long long int &end_index);

bool add_user(unsigned long long int id, string password, unsigned seed);

bool register_user(unsigned long long int id, unsigned seed);

bool valid_password(string password);

bool login(unsigned int seed, bool &is_admin,  unsigned long long int &user_id);

#endif // LOGIN_H
