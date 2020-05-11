#include "movie_room.h"
#include <windows.h>

Room::Room (int _id) {

    id = _id;

    int type;
    bool is_vibro;
    for (short int i = 0; i < 7; i++) {
        for (short int j = 0; j < 10; j++) {
            type = 0;
            is_vibro = false;

            if (((0 < i) and (i < 4)) and ((1 < j) and (j < 8))) is_vibro = true;
            else if ((3 < i) and ((j < (i - 3)) or ((12 - i) < j))) type = -1;
            else if ((4 == i) and ((2 < j) and (j < 7))) is_vibro = true;

            seats[i][j].is_vibro = is_vibro;
            seats[i][j].type = type;
        }
    }
}

void colored_display(const Seat &seat, const bool &is_admin) {

    char chr;
    if (!seat.is_empty) chr = 'X';
    else if (is_admin and seat.is_vibro) chr = char(254);
    else chr = 'O';

    if (seat.type) { //Los enteros diferentes de 0 son bool true
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

        SetConsoleTextAttribute(hConsole, (seat.type == 2)? 12: 2);
        cout << chr << endl;
        SetConsoleTextAttribute(hConsole, 7);
    }
    else cout << chr;
}

void Room::display_row(short int row, const short int &aux, const bool &is_admin) {

    cout << "    " << char(177) << char(177) << ' ';
    for (short int j = 0; j < aux; j++) cout << "    ";
    for (short int j = aux; j < (10 - aux); j++) {
        cout << char(179) << ' ';
        colored_display(seats[row][j], is_admin);
        cout << ' ';
    }
    cout << char(179);
    for (short int j = 0; j < aux; j++) cout << "    ";
}

void display_separator(unsigned short int size, short int chr1, short int chr2, short int chr3) {

    //Esta función sólo es con el propósito de ahorrar el tener que reescribir el código para imprimir
    //cada separación de las filas de la matriz, debido a que estos códigos son muy similares.

    cout << char(chr1);
    for (unsigned int i = 0; i < (size - 1u); i++) cout << char(196) << char(196) << char(196) << char(chr2);
    cout << char(196) << char(196) << char(196) << char(chr3);
}

void Room::display_seats(const bool &is_admin) {

    short int aux = 0;

    cout << endl << endl;
    cout << "    ";
    for (short int j = 0; j < 47; j++) cout << char(177);
    cout << endl;

    cout << "    " << char(177) << char(177) << ' ';
    display_separator(10, 218, 194, 191);
    cout << ' ' << char(177) << char(177) << endl;
    display_row(0, 0, is_admin);
    cout << ' ' << char(177) << char(177) << endl;

    for (short int i = 1; i < 7; i++) {

        if (3 < i) {
            cout << "    " << char(177) << char(177) << ' ';
            for (short int j = 0; j < aux; j++) cout << "    ";
            display_separator(10 - 2*aux, 192, 197, 217);
            for (short int j = 0; j < aux; j++) cout << "    ";
            cout << ' ' << char(177) << char(177) << endl;
            aux = i - 3;
        }
        else {
            cout << "    " << char(177) << char(177) << ' ';
            display_separator(10, 195, 197, 180);
            cout << ' ' << char(177) << char(177) << endl;
        }

        display_row(i, aux, is_admin);
        cout << ' ' << char(177) << char(177) << endl;
    }

    cout << "    " << char(177) << char(177) << ' ';
    for (short int j = 0; j < 3; j++) cout << "    ";
    display_separator(4, 192, 193, 217);
    for (short int j = 0; j < 3; j++) cout << "    ";
    cout << ' ' << char(177) << char(177) << endl;

    cout << "    " << char(177) << char(177);
    for (short int j = 0; j < 39; j++) cout << ' ';
    cout << "    " << char(177) << char(177) << ' ' << endl;

    cout << "    " << char(177) << char(177) << "      " << char(177);
    for (short int j = 0; j < 29; j++) cout << char(178);
    cout << char(177) << "      " << char(177) << char(177) << endl << "    ";

    for (short int j = 0; j < 47; j++) cout << char(177);
    cout << endl << endl;
}








