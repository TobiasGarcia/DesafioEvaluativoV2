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

void display_wall(short int left, short int walls, short int right) {

    for (short int j = 0; j < left; j++) cout << ' ';
    for (short int j = 0; j < walls; j++) cout << char(177);
    for (short int j = 0; j < right; j++) cout << ' ';
}

void Room::display_row(short int row, const short int &aux, const bool &is_admin) {

    for (short int j = aux; j < (10 - aux); j++) {
        cout << char(179) << ' ';
        colored_display(seats[row][j], is_admin);
        cout << ' ';
    }
    cout << char(179);
}

void display_separator(short int size, short int chr1, short int chr2, short int chr3) {

    //Esta función sólo es con el propósito de ahorrar el tener que reescribir el código para imprimir
    //cada separación de las filas de la matriz, debido a que estos códigos son muy similares.

    cout << char(chr1);
    for (int i = 0; i < (size - 1); i++) cout << char(196) << char(196) << char(196) << char(chr2);
    cout << char(196) << char(196) << char(196) << char(chr3);
}

void Room::display_seats(const bool &is_admin) {

    short int aux = 0;

    cout << endl << endl;
    display_wall(4, 47, 0); cout << endl;

    display_wall(4, 2, 1); display_separator(10, 218, 194, 191); display_wall(1, 2, 0); cout << endl;
    display_wall(4, 2, 1); display_row(0, 0, is_admin); display_wall(1, 2, 0); cout << endl;

    for (short int i = 1; i < 7; i++) {

        if (3 < i) {
            display_wall(4, 2, 1 + 4*aux); display_separator(10 - 2*aux, 192, 197, 217); display_wall(1 + 4*aux, 2, 0); cout << endl;
            aux = i - 3;
        }
        else {
            display_wall(4, 2, 1); display_separator(10, 195, 197, 180); display_wall(1, 2, 0); cout << endl;
        }
        display_wall(4, 2, 1 + 4*aux); display_row(i, aux, is_admin); display_wall(1 + 4*aux, 2, 0); cout << endl;
    }
    display_wall(4, 2, 13); display_separator(4, 192, 193, 217); display_wall(13, 2, 0); cout << endl;

    display_wall(4, 2, 22); display_wall(21, 2, 0); cout << endl;
    display_wall(4, 2, 22); display_wall(21, 2, 0); cout << endl;
    display_wall(4, 2, 22); display_wall(21, 2, 0); cout << endl;

    display_wall(4, 2, 5); cout << char(177); for (short int j = 0; j < 31; j++) cout << char(178); cout << char(177); display_wall(5, 2, 0); cout << endl;
    display_wall(4, 47, 0); cout << endl;
    cout << endl;
}








