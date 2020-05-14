#include "show.h"
#include <fstream>
#include "dialog.h"
#include <windows.h>
#include "adminSession.h"
#include <vector>
#include "login.h"

void Show::fill_row(string line, short row) {

    if (row == 0) {
        for (short int j = 0; j < 10; j++) {
            seats[0][j].is_empty = (line[3*j] == '1');
            seats[0][j].sale_type = int(line[1 + 3*j]) - 48;
        }
    }
    else if (is_in_range(row, 1, 3)) {
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

Show::Show(short int _id, bool &exists) {

    id = _id;
    fstream file("../DesafioEvaluativoV2/data/shows/show" + to_string(id) + ".txt", ios::in);
    if (file.is_open()) {

        exists = true;
        string line;

        getline(file, line); movie_name = line;
        getline(file, line); is_3D = (line == "1");
        getline(file, line); genre = line;
        getline(file, line); str2int(line, clasi);
        getline(file, line); str2int(line, hour);
        getline(file, line); str2int(line, finish_hour);
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

Show::Show(short int _id, string _movie_name, bool _is_3D, string _genre, short int _clasi, short int _hour, short int _finish_hour, short int _duration, short int _room, short int _empty_places) {

    id =_id; movie_name = _movie_name; is_3D = _is_3D; genre = _genre; clasi = _clasi; hour = _hour;
    finish_hour = _finish_hour; duration = _duration; room = _room; empty_places = _empty_places;

    for (short int row = 0; row < 4; row++) fill_row("10 10 10 10 10 10 10 10 10 10 ", row);
    fill_row("   10 10 10 10 10 10 10 10 ", 4);
    fill_row("      10 10 10 10 10 10 ", 5);
    fill_row("         10 10 10 10 ", 6);
}

void Show::save_show(short int shows_num) const {

    //No hace falta el exists porque si no está el archivo, lo crea.

    //Guarda carácteres raros pero no son problema porque lee lo que debería leer.

    fstream file("../DesafioEvaluativoV2/data/shows/shows_num.txt", ios::out);
    file << shows_num;
    file.close();

    file.open("../DesafioEvaluativoV2/data/shows/show" + to_string(id) + ".txt", ios::out);
    file << movie_name << '\n';
    file << (is_3D?'1':'0') << '\n';
    file << genre << '\n';
    file << to_string(clasi) << '\n';
    file << to_string(hour) << '\n';
    file << to_string(finish_hour) << '\n';
    file << to_string(duration) << '\n';
    file << to_string(room) << '\n';
    file << to_string(empty_places) << '\n';

    string seat;
    short int j_min = 0, j_max = 10;

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

void Show::display_row(short int row, const short int &aux, const bool &is_admin) const {

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

void Show::display_seats(const bool &is_admin) const {

    //Éste método imprime en pantalla la sala de la película.

    short int aux = 0;

    cout << endl << endl;
    display_wall(4, 49, 0); cout << endl;
    display_wall(4, 2, 23); display_wall(22, 2, 0); cout << endl;

    display_wall(4, 2, 4); for (short int i = 0; i < 9; i++) cout << ' ' << (i + 1) << "  ";
    cout << " 10"; display_wall(2, 2, 0); cout << endl;

    display_wall(4, 2, 3); display_separator(10, 218, 194, 191); display_wall(1, 2, 0); cout << endl;
    display_wall(4, 2, 1); cout << "A "; display_row(0, 0, is_admin); display_wall(1, 2, 0); cout << endl;

    for (short int i = 1; i < 7; i++) {

        if (3 < i) {
            display_wall(4, 2, 3 + 4*aux); display_separator(10 - 2*aux, 192, 197, 217); display_wall(1 + 4*aux, 2, 0); cout << endl;
            aux = i - 3;
        }
        else {
            display_wall(4, 2, 3); display_separator(10, 195, 197, 180); display_wall(1, 2, 0); cout << endl;
        }
        display_wall(4, 2, 1); cout << char(65 + i); display_wall(0, 0, 1 + 4*aux); display_row(i, aux, is_admin); display_wall(1 + 4*aux, 2, 0); cout << endl;
    }
    display_wall(4, 2, 15); display_separator(4, 192, 193, 217); display_wall(13, 2, 0); cout << endl;

    display_wall(4, 2, 23); display_wall(22, 2, 0); cout << endl;
    display_wall(4, 2, 23); display_wall(22, 2, 0); cout << endl;
    display_wall(4, 2, 23); display_wall(22, 2, 0); cout << endl;

    display_wall(4, 2, 7); cout << char(177); for (short int j = 0; j < 31; j++) cout << char(178); cout << char(177); display_wall(5, 2, 0); cout << endl;
    display_wall(4, 49, 0); cout << endl;
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
                cout << endl << "  Sorry, there is a '" << shows_num << "' in the shows_num.txt file," << endl;
                cout << "  but there is no show" << id << ".txt file in the shows folder." << endl;
                return false;
            }
        }
        return true;
    }
    else {
        cout << endl << "  Sorry, the shows_num.txt file could not be found." << endl;
        return false;
    }
}

void display_edge(short int size, short int chr) {
    for (short int i = 0; i < size; i++) cout << char(205);
    cout << char(chr);
}

void display_adapter_separator(short int chr1, short int chr2, short int chr3, short int size1, short int size2) {

    //Es muy especifica para éste problema como tal.

    cout << "   " << char(chr1);
    display_edge(4, chr2);
    display_edge(size1, chr2);
    display_edge(size2, chr2);
    display_edge(6, chr2);
    display_edge(10, chr2);
    display_edge(8, chr2);
    display_edge(7, chr2);
    display_edge(10, chr2);
    display_edge(6, chr3);
}

void centred_display(string data, const short int &size) {

    //La variable size es el espacio que ocupará todo el display.

    short int spaces = (size - data.length() - 2), aux = int(spaces/2); //Más dos espacios de los extremos

    for (short int i = 0; i < aux; i++) cout << ' ';
    cout << ' ' << data << ' ';
    for (short int i = 0; i < (spaces - aux); i++) cout << ' ';
}

void get_longest_size(const vector<Show> &shows, short int &size1, short int &size2) {

    //Es muy especifica para éste problema como tal.
    //Devuelve en size1 la longitd del mayor nombre, en size2
    //la del mayor genero.

    //Porque mínimo es el de 'Name' y el de 'Genre'.

    size1 = 4;
    size2 = 5;
    short int len = shows.size();
    for (short int i = 0; i < len; i++) {
        if (size1 < short(shows.at(i).get_movie_name().length())) size1 = short(shows.at(i).get_movie_name().length());
        if (size2 < short(shows.at(i).get_genre().length())) size2 = short(shows.at(i).get_genre().length());
    }
}

void Show::display_show(const short int &size1, const short int &size2) const {


    //Pasamos de horario militar al convencional, AMIGBALES, no funciona con el -1.
    string str_hour = (hour == 0)?"12":to_string(1 + (hour-1)%12);
    str_hour += (hour < 12)?":00 am":":00 pm";

    cout << "   " << char(186); centred_display(to_string(id + 1), 4);
    cout << char(186); centred_display(movie_name, size1);
    cout << char(186); centred_display(genre, size2);
    cout << char(186); centred_display(is_3D?"Yes":"No", 6);
    cout << char(186); centred_display(to_string(duration) + " mins", 10);
    cout << char(186); centred_display(to_string(clasi) + "+", 8);
    cout << char(186); centred_display(to_string(empty_places), 7);
    cout << char(186); centred_display(str_hour, 10);
    cout << char(186); centred_display(to_string(room), 6);
    cout << char(186);
}

void msg_and_cls(string msg) {
    cout << "  " << msg << endl << endl << "  ";
    system("pause");
    system("cls");
}

bool Show::get_index(short &index, bool is_row, const bool &is_admin, short int row) {

    bool ask = true;
    string ans, msg;

    if (is_admin) msg = "  Which seat do you want to offer?";
    else msg = "  Which seat do you want to reserve?";

    system("cls");

    //Delgados
    //218 196 194 196 191
    //179     179     179
    //195 196 197 196 180
    //179     179     179
    //192 196 193 196 217

    //Gruesos
    //201 205 203 205 187
    //186     186     186
    //204 196 206 196 185
    //186     186     186
    //200 205 202 205 188

    while (ask) {

        display_seats(is_admin);
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

        cout << "  -------------------------------LEGEND-------------------------------" << endl;
        cout << "  O: Empty seat    X: Sold seat      ";

        if (is_admin) cout << char(254) << ": Seat with VibroSound technology" << endl << endl;
        else cout << endl << endl;

        cout << "  The color means in which modality the seat is being offered:" << endl;
        cout << "  ";

        SetConsoleTextAttribute(hConsole, 15);
        cout << "White: General";
        SetConsoleTextAttribute(hConsole, 7);
        cout << " ---- ";
        SetConsoleTextAttribute(hConsole, 12);
        cout << "Red: VibroSound";
        SetConsoleTextAttribute(hConsole, 7);
        cout << " ---- ";
        SetConsoleTextAttribute(hConsole, 6);
        cout << "Yellow: Gold";
        SetConsoleTextAttribute(hConsole, 7);
        cout << endl << endl << msg << endl;

        //Mostramos la fila si ya estamos en la columna.

        if (!is_row) cout << endl << "  CURRENT ROW: " << char(row + 65) << endl;

        cout << "  Enter the " << (is_row?"row":"column") << " of the seat, or an empty space for return: (" << (is_row?'A':'1') << " - " << (is_row?"G":"10") << ") ";
        fflush(stdin);
        getline(cin, ans);

        if ((ans == "") and yes_no_question("You want to return? (Enter 'Yes' or 'No')")) return false;
        else if (ans == "") system("cls");
        else if (is_row and ((1 < ans.length()) or (!is_in_range(ans[0], 65, 71) and !is_in_range(ans[0], 97, 103)))) {
            msg_and_cls("Sorry, the row must be a letter between A and G.");
        }
        else if (ans == "10") {
            ask = false;
            index = 9;
        }
        else if (!is_row and ((1 < ans.length()) or !is_in_range(ans[0], 49, 57))) {//Es suficiente con 1 < len pues si fuera 0 habría saldio por la primera sentencia del if.
            msg_and_cls("Sorry, the column must be a number between 1 and 10.");
        }
        else {
            ask = false;
            if (is_row and (ans[0] < 72)) index = short(ans[0]) - 65; //Mayúscula.
            else if (is_row) index = short(ans[0]) - 97; //Minúscula.
            else index = short(ans[0]) - 49; //Número de la columna.
        }
    }
    return true;
}

void Show::offer_seats(short int shows_num) {

    string msg;
    bool ask = true;
    short int row, column;

    while (ask and get_index(row, true, true) and get_index(column, false, true, row)) {

        if (!seats[row][column].is_empty) {
            cout << endl;
            msg = "Sorry, the seat ";
            msg.push_back(char(row + 65));
            msg.append(to_string(column + 1));
            msg.append(" is sold, so we can't change the way it is offered");
            msg_and_cls(msg);
        }
        else if (seats[row][column].sale_type == -1) {
            cout << endl;
            msg_and_cls("Sorry, that is not a seat");
        }
        else if (!seats[row][column].is_vibro) {
            cout << endl;
            msg = "Sorry, the seat ";
            msg.push_back(char(row + 65));
            msg.append(to_string(column + 1));
            msg.append(" doesn't have VibroSound technology,\n  so it can only be offerd in General");
            msg_and_cls(msg);
        }
        else {
            cout << endl << "  We can offer the seat " << char(row + 65) << (column + 1) << " in the modalities:" << endl;
            cout << "  1. General     2. VibroSound     3. Gold" << endl << "  ";
            //-1 para poder pasar a 0, 1 o 2.
            seats[row][column].sale_type = get_int_input("In which of them do you want to offer it? (1 - 3)", "\nSorry, we only have these 3 modalities:\n  1. General     2. VibroSound     3. Gold", 1, 3) - 1;
            cout << endl;
            msg_and_cls("The offer has been successfully placed!");
            display_seats(true);
            ask = yes_no_question("Do you want to offer other seat? (Enter 'Yes' for offer other seat or 'No' for exit)");
        }
    }
    save_show(shows_num);
}

short int explain_offer_types(const Seat &seat, const short int &row, const short int &column) {

    short int price;
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    if (seat.sale_type == 2) {
        price = 19900;
        cout << "  The seat " << char(row + 65) << (column + 1) << " is being offered in ";

        SetConsoleTextAttribute(hConsole, 6);
        cout << "Gold";
        SetConsoleTextAttribute(hConsole, 7);

        cout << endl << endl << "  It means that you can enjoy of the VibroSound service and you can choose one" << endl;
        cout << "  of the cafeteria combos that will be brought to your seat before the movie." << endl;
    }
    else if (seat.sale_type == 1) {
        price = 10900;
        cout << "  The seat " << char(row + 65) << (column + 1) << " is being offered in ";

        SetConsoleTextAttribute(hConsole, 12);
        cout << "VibroSound";
        SetConsoleTextAttribute(hConsole, 7);

        cout << endl << endl << "  It means that you can enjoy of the VibroSound service" << endl;
        cout << "  while you see the movie." << endl;
    }
    else {
        price = 8700;
        cout << "  The seat " << char(row + 65) << (column + 1) << " is being offered in General";
        cout << endl << endl << "  It means that you can enjoy the movie on a normal seat." << endl;
    }
    return price;
}

void Show::reserve_seat(short shows_num, const unsigned long long int &user_id, const unsigned int &seed) {

    bool ask = true;
    unsigned int price;
    short int row, column, combo = 0;

    while (ask and get_index(row, true, false) and get_index(column, false, false, row)) {

        if (!seats[row][column].is_empty) {
            msg_and_cls("Sorry, the seat " + to_string(char(row + 65)) + to_string(column + 1) + " is already reserved");
        }
        else if (seats[row][column].sale_type == -1) {
            msg_and_cls("Sorry, that is not a seat");
        }
        else {

            system("cls");
            display_seats(false);

            price = explain_offer_types(seats[row][column], row, column);

            if (is_3D) {
                cout << endl << "  It costs $" << price << ", plus $3000 for the 3D show, for a total of $" << (price + 3000);
                price += 3000;
            }
            else cout << endl << "  It costs $" << price << '.';

            if (yes_no_question("You want to reserve that seat? (Enter 'Yes' or 'No')")) {
                system("cls");
                ask = false;
                cout << endl << "  Reserve seat " << char(row + 65) << (column + 1) << " ----- $" << price << endl;
                if (charge_money(price)) {
                    seats[row][column].is_empty = false;
                    empty_places--;
                    system("cls");

                    if (seats[row][column].sale_type == 2) combo = offer_combos();
                    update_user(user_id, hour, room, row, column, combo, seed);
                }
                else {
                    cout << endl << "  You canceled the reservation" << endl << endl << "  ";
                    system("pause");
                    return;
                }
            }
            else system("cls");
        }
    }
    save_show(shows_num);
}

void display_labels(const short int &size1, const short int &size2) {

    display_adapter_separator(201, 203, 187, size1, size2); cout << endl;
    cout << "   " << char(186); centred_display("ID", 4);
    cout << char(186); centred_display("Name", size1);
    cout << char(186); centred_display("Genre", size2);
    cout << char(186); centred_display("3D", 6);
    cout << char(186); centred_display("Duration", 10);
    cout << char(186); centred_display("Class.", 8);
    cout << char(186); centred_display("Seats", 7);
    cout << char(186); centred_display("Hour", 10);
    cout << char(186); centred_display("Room", 6);
    cout << char(186);
}

void display_shows(const vector<Show> &shows) {

    short int len = shows.size(), size1, size2;
    get_longest_size(shows, size1, size2);
    size1 += 2;//Añadimos los espacios vacios de los extremos.
    size2 += 2;

    cout << endl;
    display_labels(size1, size2);
    cout << endl;

    for (short int i = 0; i < len; i++) {
        display_adapter_separator(204, 206, 185, size1, size2); cout << endl;
        shows.at(i).display_show(size1, size2); cout << endl;
    }

    display_adapter_separator(200, 202, 188, size1, size2); cout << endl;
}

bool is_room_available(const vector<Show> &shows, const short int &room, const short int &hour, short int finish_hour, short int &show_hour, short int &show_finish_hour) {

    short int len = shows.size();
    for (short int i = 0; i < len; i++) {
        show_hour = shows.at(i).get_hour();
        show_finish_hour = shows.at(i).get_finish_hour();
        if ((shows.at(i).get_room() == room) and ((hour < show_finish_hour) and (show_hour < finish_hour))) return false;
    }
    return true;
}

void add_show(vector<Show> &shows) {

    bool is_3D, ask = true;
    string movie_name, genre, ans;
    short int clasi, hour, finish_hour, duration, room, show_hour, show_finish_hour;

    display_shows(shows);

    movie_name = get_non_empty_line("Enter the name of the movie:");
    is_3D = yes_no_question("The movie is in 3D? (Enter 'Yes' if it is, or 'No' otherwise)");
    genre = get_non_empty_line("Enter the genre of the movie:");

    //Técnicamente en todos las warnings siguintes debería decir '...must be a positive integer...', pero
    //considero que eso no sería amigable con el usuario, por lo cual lo dejamos en '...must be a number...'.

    clasi = get_int_input("Enter the classification of the movie:", "Sorry, the classification must be a number.", 0);
    hour = get_int_input("At what time will the movie be shown? (In 24 hours format)", "Sorry, the hour must be a number between 0 and 23.", 0, 23);
    duration = get_int_input("Enter the duration of the movie: (In minutes)", "Sorry, the duration must be a number.", 1);
    finish_hour = hour + ceiling(float(duration)/60.0f);
    room = get_int_input("In which of the four rooms will the movie be shown? (Enter the room number)", "Sorry, the room number must be a number between 1 and 4.", 1, 4);

    while (ask and !is_room_available(shows, room, hour, finish_hour, show_hour, show_finish_hour)) {
        system("cls");
        display_shows(shows);
        cout << endl << "  Sorry, room " << room << " is reserved from " << show_hour << ":00 to " << show_finish_hour << ":00, and you need to reserved" << endl;
        cout << "  the room " << finish_hour - hour << " hour(s), because your movie lasts " << duration << " minutes." << endl << endl;
        cout << "  Do you want to change the room or the hour?";
        ask = yes_no_question("Enter 'Yes' for change they, or 'No' for cancel the show)");

        if (ask) {

            system("cls");
            display_shows(shows);
            hour = get_int_input("At what time will the movie be shown? (In 24 hours format)", "Sorry, the hour must be a number between 0 and 23.", 0, 23);
            finish_hour = hour + ceiling(float(duration)/60.0f);
            room = get_int_input("In which of the four rooms will the movie be shown? (Enter the room number)", "Sorry, the room number must be a number between 1 and 4.", 1, 4);
        }
    }

    if (ask) {
        Show show(shows.size(), movie_name, is_3D, genre, clasi, hour, finish_hour, duration, room, 58);
        shows.push_back(show);
    }
}


























