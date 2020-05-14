#include <iostream>
#include "dialog.h"
#include "login.h"
#include "show.h"
#include "session.h"
#include <array>
#include "code_decode.h"

using namespace std;

//ONLY FOR WINDOWS

//La semilla de codificación por defecto es 12,
//pero puede ser cualquier otro entero positivo.

int main() {

    //Inicializamos las variables.

    unsigned int code_seed = 12;
    bool is_admin, logged, on = true;
    unsigned long long int user_id, total;

    array<unsigned int, 6> sales;
    get_sales_record(sales, total, 12);

    vector<Show> shows;
    get_shows(shows);

    while (on) {

        display_title(false);
        logged = login(code_seed, is_admin, user_id);

        if (logged and is_admin) {
            system("cls");
            admin_session(shows, sales, total, code_seed, on);
        }
        else if (logged) {
            system("cls");
            user_session(shows, sales, total, user_id, code_seed);
        }
    }

    system("cls");
    cout << endl << "             Good Bye!" << endl << endl;

    return 0;
}
