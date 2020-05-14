#ifndef ADMINSESSION_H
#define ADMINSESSION_H

#include "show.h"

void admin_session(vector<Show> &shows, const unsigned int &code_seed, bool &on);
bool charge_money(unsigned int price);
void user_session(vector<Show> &shows, const unsigned long long int &user_id, const unsigned int &seed);
short int offer_combos();

#endif // ADMINSESSION_H
