#include "show.h"
#include <fstream>
#include "dialog.h"
#include <windows.h>
#include <vector>

void Show::fill_row(string line, short row) {

    if (row == 0) {
        for (short int j = 0; j < 10; j++) {
            seats[0][j].is_empty = (line[3*j] == '1');
            seats[0][j].sale_type = int(line[1 + 3*j]) - 48;
        }
    }
    else if ((0 < row) and (row < 4)) {
        for (short int j = 0; j < 10; j++) {
            seats[row][j].is_empty = (line[3*j] == '1');
            seats[row][j].sale_type = int(line[1 + 3*j]) - 48;
            if ((1 < j) and (j < 8)) seats[row][j].is_vibro = true;
        }
    }
    else if (row == 4){
        for (short int j = 1; j < 9; j++) {
            seats[4][j].is_empty = (line[3*j] == '1');
            seats[4][j].sale_type = int(line[1 + 3*j]) - 48;
            if ((2 < j) and (j < 7)) seats[4][j].is_vibro = true;
        }
    }
    else {
        for (short int j = (row - 3); j < (13 - row); j++) {
            seats[row][j].is_empty = (line[3*j] == '1');
            seats[row][j].sale_type = int(line[1 + 3*j]) - 48;
        }
    }
}

Show::Show(short int id, bool &exists) {

    fstream file("../DesafioEvaluativoV2/data/shows/show" + to_string(id) + ".txt", ios::in);
    if (file.is_open()) {

        exists = true;
        string line;

        getline(file, line); movie_name = line;
        getline(file, line); if (line == "1") is_3D = true;
        getline(file, line); genre = line;
        getline(file, line); str2int(line, clasi);
        getline(file, line); str2int(line, hour);
        getline(file, line); str2int(line, duration);
        getline(file, line); str2int(line, room);
        getline(file, line); str2int(line, empty_places);

        for (short int row = 0; row < 7; row++) {
            getline(file, line);
            fill_row(line, row);
        }
        file.close();
    }
    else exists = false;
}

Show::Show(string _movie_name, bool _is_3D, string _genre, short int _clasi, short int _hour, short int _duration, short int _room, short int _empty_places) {

    movie_name = _movie_name; is_3D = _is_3D; genre = _genre; clasi = _clasi;
    hour = _hour; duration = _duration; room = _room; empty_places = _empty_places;

    for (short int row = 0; row < 4; row++) fill_row("10 10 10 10 10 10 10 10 10 10 ", row);
    fill_row("10 10 10 10 10 10 10 10 ", 4);
    fill_row("10 10 10 10 10 10 ", 5);
    fill_row("10 10 10 10 ", 6);
}

void Show::save_show(short int id) {

    //No hace falta el exists porque si no está el archivo, lo crea.

    fstream file("../DesafioEvaluativoV2/data/shows/show" + to_string(id) + ".txt", ios::in);
    file << movie_name << '\n';
    file << (is_3D?'1':'0') << '\n';
    file << genre << '\n';
    file << to_string(clasi) << '\n';
    file << to_string(hour) << '\n';
    file << to_string(duration) << '\n';
    file << to_string(room) << '\n';
    file << to_string(empty_places) << '\n';

    string seat;
    short int j_min, j_max;

    for (short int i = 0; i < 7; i++) {
        if (3 < i) {
            j_min = i - 3;
            j_max = 13 - i;
            for (short int j = 0; j < j_min; j++) file << "   ";
        }
        for (short int j = j_min; j < j_max; j++) {
            seat = "";
            seat.push_back((seats[i][j].is_empty)?'1':'0');
            seat.push_back(char(seats[i][j].sale_type + 48));
            seat.push_back(' ');
            file << seat;
        }
        file << '\n';
    }
    file.close();
}

//Ahora vamos con la parte de imprimir la información del matriz seats
//en pantalla.

void display_wall(short int left, short int walls, short int right) {

    //Esto es solo para imprimir una pared del cine, left determina cuantos espacios vacíos dejamos
    //a la izquierda, walls la cantidad de espacios, o más bien el espasor de la pared, y right
    //la cantidad de espacios vacíos luego de la pared.

    for (short int j = 0; j < left; j++) cout << ' ';
    for (short int j = 0; j < walls; j++) cout << char(177);
    for (short int j = 0; j < right; j++) cout << ' ';
}

void colored_display(const Seat &seat, const bool &is_admin) {

    //Los asientos ocupados serán impresos con una X, mientras que los disponibles
    //con un O. Si el admin es el que desea ver la sala de cine, el tiene que poder
    //identificar cuales sillas cuentan con tecnología VibroSound, por lo cual
    //si is_admin es true, las sillas que cuentann con VibroSound son impresas
    //con un ■ en caso de no estar vendidas aún. Por otra parte,
    //independientemente de si está ocuapada o no, cuenta con
    //VibroSound o no, el color determina en que forma se está
    //ofertando la silla a los usuarios, blanco es en General,
    //rojo es en VibroSound, y amarillo para Gold.

    //Recordemos que una silla con tecnología VibroSound puede ser ofrecida como
    //General, VibroSound o Gold, mientras que una silla sin tecnología VibroSound
    //sólo puede ser ofertada en General.

    char chr;
    if (!seat.is_empty) chr = 'X';
    else if (is_admin and seat.is_vibro) chr = char(254); //Éste carácter es el ■.
    else chr = 'O';

    //Recordemos que los enteros diferente de 0 siempre arroja true al ser juzgados
    //dentro de un condicional, luego el if() se cumplira si el asiento se oferta
    //en 1 (VibroSound) o 2 (Gold), por lo cual le debemos cambiar el color.

    if (seat.sale_type) {

        //Honestamente éste código lo tuve que copiar y pegar de internet,
        //más especificamente de éste foro:

        //https://stackoverflow.com/questions/4053837/colorizing-text-in-the-console-with-c

        //Se utiliza para poder manejar colores en consola.

        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

        SetConsoleTextAttribute(hConsole, (seat.sale_type == 2)?6:12);
        cout << chr;
        SetConsoleTextAttribute(hConsole, 7);
    }
    else cout << chr;
}

void Show::display_row(short int row, const short int &aux, const bool &is_admin) {

    //Éste método imprime en pantalla la fila row de la matriz seats, aux es para
    //identifiacar si hay que mostrar espacios antes de los asientos como sucede
    //en las filas 4, 5 y 6. La variable is_admin es para saber si es el admin
    //quien solicita ver la sala de cine, es para poder imprimir los asientos
    //con tecnología VibroSound con los ■s.

    for (short int j = aux; j < (10 - aux); j++) {
        cout << char(179) << ' ';
        colored_display(seats[row][j], is_admin);
        cout << ' ';
    }
    cout << char(179);
}

void display_separator(short int size, short int chr1, short int chr2, short int chr3) {

    //Ésta función sólo es con el propósito de ahorrar el tener que reescribir el código para imprimir
    //cada separación de las filas de la sala de cine, debido a que estos códigos solo difieren en
    //los valores de algunos carácteres.

    cout << char(chr1);
    for (int i = 0; i < (size - 1); i++) cout << char(196) << char(196) << char(196) << char(chr2);
    cout << char(196) << char(196) << char(196) << char(chr3);
}

void Show::display_seats(const bool &is_admin) {

    //Éste método imprime en pantalla la sala de la película.

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

bool get_shows(vector<Show> &shows) {

    fstream file("../DesafioEvaluativoV2/data/shows/shows_num.txt", ios::in);
    if (file.is_open()) {

        bool exists;
        string line;
        short int shows_num;

        getline(file, line);
        str2int(line, shows_num);

        file.close();

        shows.reserve(shows_num);
        for (short int id = 0; id < shows_num; id++) {
            Show show(id, exists);
            if (exists) shows.push_back(show);
            else {
                cout << endl << "Sorry, there is a '" << shows_num << "' in the shows_num.txt file," << endl;
                cout << "but there is no show" << id << ".txt file in the shows folder." << endl;
                return false;
            }
        }
        return true;
    }
    else {
        cout << endl << "Sorry, the shows_num.txt file could not be found." << endl;
        return false;
    }
}


























