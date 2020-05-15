#ifndef ADMINSESSION_H
#define ADMINSESSION_H

#include "show.h"
#include <array>
#include <iostream>

using namespace std;

void user_session(vector<Show> &shows, array<unsigned int, 6> &sales, unsigned long long int &total, const unsigned long long int &user_id, const unsigned int &seed);

bool update_user(const unsigned long long int &user_id, const short int &hour, const short int &room,
                 const short int &row, const short int &column, const short int &combo, const unsigned int &seed);

void display_admin_menu();

void admin_session(vector<Show> &shows, array<unsigned int, 6> &sales, unsigned long long int &total, const unsigned int &code_seed, bool &on);

void schedule_show(vector<Show> &shows);

bool get_sales_record(array<unsigned int, 6> &sales, unsigned long long int &total, const unsigned int &seed);

void sales_record_line(string &text, string sale_type, unsigned int price, unsigned long long int num);

void save_sales(const array<unsigned int, 6> &sales, const unsigned long long int &total,const unsigned int &seed);

void reset_sales_record(array<unsigned int, 6> &sales, unsigned long long int &total, unsigned int seed);

#endif // ADMINSESSION_H
