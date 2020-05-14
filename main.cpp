#include <iostream>
#include "dialog.h"
#include "login.h"
#include "show.h"
#include "adminSession.h"
//#include "code_decode.h"

using namespace std;

//--------------------------------------NO OLVIDAR RESTAR EMPTY SEATS

int main() {

    //code_file("../DesafioEvaluativo/data/nat.txt", "../DesafioEvaluativo/data/sudo.txt", 12);

//    La semilla de codificación por defecto es 12,
//    pero puede ser cualquier otro entero positivo.

    unsigned long long int user_id;
    unsigned int code_seed = 12;
    bool on = true, is_admin, logged;

    vector<Show> shows;
    get_shows(shows);

    while (on) {
        display_welcome(false);
        logged = login(code_seed, is_admin, user_id);
        if (logged and is_admin) {
            system("cls");
            admin_session(shows, code_seed, on);
        }
        else if (logged) {
            system("cls");
            user_session(shows, user_id, code_seed);
        }
    }
    system("cls");
    cout << endl << "             Good Bye!" << endl << endl;
    return 0;
}
