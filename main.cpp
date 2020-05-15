#include <iostream>
#include <vector>
#include <array>
#include "session.h"
#include "dialog.h"
#include "login.h"

using namespace std;

//NOTA IMPORTANTE SOBRE COMPATIBILIDAD:

//Este programa fue desarrollado y testeado utilziando el sistema operativo Windows 10,
//por lo cual se da garantía de su correcto funcionamiento bajo este mismo sistema operativo,
//pues se utilizó una libreria en particular para agregar colores a la consola de Windows,
//la libreria windows.h, y dos instrucciones del propio sistema, 'cls' y 'pause'.

//NOTA SOBRE LA CODIFIACIÓN DE ARCHIVOS:

//El método de codificación utilizado sobre los archivos fue el método 1 expuesto en la
//práctica 3 del laboratorio, con n = 3 (semilla igual a 3). Es importante no cambiar
//esta semilla pues los archivos con los que viene por defecto el programa fueron
//codificados con ella.

int main() {

    //Inicializamos algunas variables que requiere el programa, para más información
    //ver el vídeo de Youtube o leer la documentación de las funciones y métodos.

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
