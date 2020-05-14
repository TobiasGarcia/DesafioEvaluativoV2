#ifndef LOGIN_H
#define LOGIN_H

#include <iostream>

using namespace std;

bool login_admin(unsigned int seed);

bool add_user(unsigned long long int id, string password, unsigned seed);

bool search_id(string text, unsigned long long int len, unsigned long long int id, unsigned long long int &index, unsigned long long int &end_index);

bool valid_password(string password);

bool register_user(unsigned long long int id, unsigned seed);

bool update_user(const unsigned long long int &user_id, const short int &hour, const short int &room,
                 const short int &row, const short int &column, const short int &combo, const unsigned int &seed);

bool login(unsigned int seed, bool &is_admin, unsigned long long int &user_id);

#endif // LOGIN_H
