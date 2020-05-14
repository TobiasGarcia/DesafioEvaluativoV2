#ifndef ADMINSESSION_H
#define ADMINSESSION_H

#include "show.h"
#include <array>

void admin_session(vector<Show> &shows, array<unsigned int, 6> &sales, unsigned long long int &total, const unsigned int &code_seed, bool &on);
void user_session(vector<Show> &shows, array<unsigned int, 6> &sales, unsigned long long int &total, const unsigned long long int &user_id, const unsigned int &seed);
bool get_sales_record(array<unsigned int, 6> &sales, unsigned long long int &total, const unsigned int &seed);
void save_sales(const array<unsigned int, 6> &sales, const unsigned long long int &total, const unsigned int &seed);
void reset_sales_record(array<unsigned int, 6> &sales, unsigned long long int &total, unsigned int seed);
void display_sales(const array<unsigned int, 6> &sales, const unsigned long long int &total);

#endif // ADMINSESSION_H
