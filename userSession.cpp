#include "userSession.h"
#include "dialog.h"

void user_session(vector<Show> shows) {

    short int id;
    if (get_show_id(shows, id, false)) shows.at(id).reserve_seat(shows.size());
}
