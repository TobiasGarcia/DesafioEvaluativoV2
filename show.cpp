#include "show.h"
#include <fstream>
#include "dialog.h"
#include <windows.h>
#include "session.h"
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

        if (!is_admin) {
            cout << endl << endl << "  If you don't know what the General, VibroSound or Gold modalities consist of," << endl;
            cout << "  choose a seat with the modality that interests you to know what it is.";
        }

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

void Show::modify_offers(short int shows_num) {

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

void Show::reserve_seat(short shows_num, array<unsigned int, 6> &sales, unsigned long long int &total, const unsigned long long int &user_id, const unsigned int &seed) {

    string msg;
    bool ask = true;
    unsigned int price;
    short int row, column, combo = 0;

    while (ask and get_index(row, true, false) and get_index(column, false, false, row)) {

        if (!seats[row][column].is_empty) {
            msg = "Sorry, the seat ";
            msg.push_back(char(row + 65));
            msg.append(to_string(column + 1));
            msg.append(" is already reserved");
            msg_and_cls(msg);
        }
        else if (seats[row][column].sale_type == -1) msg_and_cls("Sorry, that is not a seat");
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

                    total += price;
                    if (is_3D) sales.at(seats[row][column].sale_type)++;
                    else sales.at(3 + seats[row][column].sale_type)++;

                    save_sales(sales, total, seed);

                    system("cls");

                    if (seats[row][column].sale_type == 2) {
                        combo = offer_combos();
                        system("cls");
                    }
                    update_user(user_id, hour, room, row, column, combo, seed);

                    cout << endl << "  Thank you for choosing us, we hope you enjoy your movie! :D" << endl << endl << "  ";
                    system("pause");
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


























