#include <iostream>
#include "dialog.h"
#include "login.h"
#include "movie_room.h"
//#include "code_decode.h"

using namespace std;

int main() {

    Room room(0, "../DesafioEvaluativoV2/data/rooms/room1.txt");

    room.display_seats(true);
    room.save_room("../DesafioEvaluativoV2/data/rooms/room1.txt");


    //code_file("../DesafioEvaluativo/data/nat.txt", "../DesafioEvaluativo/data/sudo.txt", 12);

//    display_welcome();

////    La semilla de codificación por defecto es 12,
////    pero puede ser cualquier otro entero positivo.

//    unsigned int code_seed = 12;
//    bool on = true, is_admin, logged;

//    while (on) {
//        logged = login(code_seed, is_admin);
//        if (logged and is_admin) {
//            cout << endl << "  ADMIN LOGEUADO!" << endl << endl;
//            cout << "  "; system("pause");
//        }
//        else if (logged) {
//            cout << endl << "  USER LOGUEADO!" << endl << endl;
//            cout << "  "; system("pause");
//        }
//        display_welcome();
//    }

    return 0;
}
