#include "movie_room.h"
#include <fstream>
#include <windows.h>
#include "dialog.h"

Room::Room (int _id, string path) {

    id = _id;

    string line;
    short int i = 0;

    fstream file(path, ios::in);

    bool bol;
    short int a;

    while (i < 7) {

        getline(file, line);
        if (i == 0) {
            for (short int j = 0; j < 10; j++) {
                bol = line[3*j] == '1';
                a = int(line[1 + 3*j]) - 48;
                seats[i][j].is_empty = bol;
                seats[i][j].sale_type = a;
            }
        }
        else if ((0 < i) and (i < 4)) {
            for (short int j = 0; j < 10; j++) {
                bol = line[3*j] == '1';
                a = int(line[1 + 3*j]) - 48;
                seats[i][j].is_empty = bol;
                seats[i][j].sale_type = a;
                if ((1 < j) and (j < 8)) seats[i][j].is_vibro = true;
            }
        }
        else if (i == 4){
            for (short int j = 1; j < 9; j++) {
                bol = line[3*(j - 1)] == '1';
                a = int(line[1 + 3*(j - 1)]) - 48;
                seats[i][j].is_empty = bol;
                seats[i][j].sale_type = a;
                if ((2 < j) and (j < 7)) seats[i][j].is_vibro = true;
            }
        }
        else {
            for (short int j = (i - 3); j < (13 - i); j++) {
                bol = (line[3*(j + 3 - i)] == '1');
                a = int(line[1 + 3*(j + 3 - i)]) - 48;
                seats[i][j].is_empty = bol;
                seats[i][j].sale_type = a;
            }
        }
        i++;
    }
    getline(file, line);
    str2int(line, empty_places);
    file.close();
}

void Room::save_room(string path) {

    string seat;
    short int j_min = 0, j_max = 10;

    fstream file(path, ios::out);
    for (short int i = 0; i < 7; i++) {

        if (3 < i) {
            j_min = i - 3;
            j_max = 13 - i;
        }

        for (short int j = j_min; j < j_max; j++) {
            seat = "";
            seat.push_back(seats[i][j].is_empty?'1':'0');
            seat.push_back(char(seats[i][j].sale_type + 48));
            seat.push_back(' ');
            file << seat;
        }
        file << '\n';
    }
    file << empty_places;
    file.close();
}

void colored_display(const Seat &seat, const bool &is_admin) {

    char chr;
    if (!seat.is_empty) chr = 'X';
    else if (is_admin and seat.is_vibro) chr = char(254);
    else chr = 'O';

    if (seat.sale_type) { //Los enteros diferentes de 0 son bool true
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

        SetConsoleTextAttribute(hConsole, (seat.sale_type == 2)?6:12);
        cout << chr;
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








