#include "adminSession.h"
#include "dialog.h"

void display_admin_menu() {
    display_welcome(true);
    cout << endl << endl << "  What you want to do?" << endl;
    cout << endl << "  1. Schedule new show" << endl;
    cout << "  2. Offer seats at scheduled shows" << endl;
    cout << "  3. Make sales report of today" << endl;
    cout << "  4. Decode file" << endl;
    cout << "  5. Turn off the application" << endl;
    cout << "  6. Exit" << endl;

}

void admin_session(vector<Show> shows, bool &on) {

    on = false;//ELIMINAR ESTO
    short int opt, id;
    display_admin_menu();
    opt = get_int_input("What you want to do?", "Sorry, that is not a valid option", short(1), short(6));

    //Siempre he preferido el if en lugar de switch.
    if (opt == 1) {
        system("cls");
        add_show(shows);
    }
    else if (opt == 2) {
        system("cls");
        if (get_show_id(shows, id, true)) shows.at(id).offer_seats(shows.size());
    }
}
