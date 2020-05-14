#include "session.h"
#include "code_decode.h"
#include "dialog.h"
#include "login.h"
#include <windows.h>
#include <array>
#include <fstream>

void display_admin_menu() {
    display_title(true);
    cout << endl << endl << "  What you want to do?" << endl;
    cout << endl << "  1. Schedule new show" << endl;
    cout << "  2. Offer seats at scheduled shows" << endl;
    cout << "  3. Make sales report of today" << endl;
    cout << "  4. Reset sales record" << endl;
    cout << "  5. Encode file" << endl;
    cout << "  6. Decode file" << endl;
    cout << "  7. Turn off the application" << endl;
    cout << "  8. Exit" << endl;
}

void admin_session(vector<Show> &shows, array<unsigned int, 6> &sales, unsigned long long int &total, const unsigned int &code_seed, bool &on) {

    short int opt = 1, id;

    while (opt != 8) {

        display_admin_menu();
        opt = get_int_input("What you want to do?", "Sorry, that is not a valid option", short(1), short(8));

        //Siempre he preferido el if en lugar de switch.

        if (opt == 1) {
            system("cls");
            add_show(shows);
        }
        else if (opt == 2) {
            system("cls");
            if (get_show_id(shows, id, true)) shows.at(id).modify_offers(shows.size());
        }
        else if (opt == 3) display_sales(sales, total);
        else if ((opt == 4) and login_admin(code_seed)) reset_sales_record(sales, total, code_seed);
        else if ((opt == 5) and login_admin(code_seed)) {
            system("cls");
            string file_nat, file_code;
            get_files_names(file_nat, file_code, true);
            code_file("../DesafioEvaluativoV2/data/" + file_nat, "../DesafioEvaluativoV2/data/" + file_code, code_seed);
        }
        else if ((opt == 6)  and login_admin(code_seed)) {
            system("cls");
            string file_nat, file_code;
            get_files_names(file_code, file_nat, false);
            decode_file("../DesafioEvaluativoV2/data/" + file_code, "../DesafioEvaluativoV2/data/" + file_nat, code_seed);
        }
        else if (opt == 7) {
            on = false;
            opt = 8;
        }
    }
}

void user_session(vector<Show> &shows, array<unsigned int, 6> &sales, unsigned long long int &total, const unsigned long long int &user_id, const unsigned int &seed) {

    //Esta función es muy pequeña pero es para no sobrecargar mucho el main y mantenerlo ordenado.
    //Ramificar en las librearias

    short int id;
    if (get_show_id(shows, id, false)) shows.at(id).reserve_seat(shows.size(), sales, total, user_id, seed);
}

//Para mostrar variedad en el manejo de los contenedores

bool get_sales_record(array<unsigned int, 6> &sales, unsigned long long int &total, const unsigned int &seed) {

    string text;
    unsigned long long int len, index = 62, end_index, aux;
    if (get_text("../DesafioEvaluativoV2/data/sales_record.txt", text, len)) {

        decode(text, len, seed);

        for (short int i = 0; i < 6; i++) {
            end_index = text.find(' ', index);
            str2int(text.substr(index, end_index - index), aux);
            sales.at(i) = aux;
            index = text.find('\n', end_index);
            index = text.find(' ', index) + 1;
        }

        index++;
        end_index = text.find('\n', index);
        str2int(text.substr(index, end_index - index), aux);
        total = aux;

        return true;
    }
    else {
        cout << "  Sorry, the sales_record.txt file could not be opened" << endl << endl;
        return false;
    }
}

void sales_record_line(string &text, string sale_type, unsigned int price, unsigned long long int num) {
    text.append(sale_type);
    text.append(": ");
    text.append(to_string(num));
    text.append(" --- $");
    text.append(to_string(num*price));
    text.push_back('\n');
}

void save_sales(const array<unsigned int, 6> &sales, const unsigned long long int &total, const unsigned int &seed) {

    string text;

    text = "TODAY'S SALES RECORD:\n----------------------------3D\n";
    sales_record_line(text, "General", 11700, sales.at(0));
    sales_record_line(text, "VibroSound", 13900, sales.at(1));
    sales_record_line(text, "Gold", 22900, sales.at(2));

    text.append("----------------------------2D\n");
    sales_record_line(text, "General", 8700, sales.at(3));
    sales_record_line(text, "VibroSound", 10900, sales.at(4));
    sales_record_line(text, "Gold", 19900, sales.at(5));

    text.append("----------------------Total: $");
    text.append(to_string(total));
    text.push_back('\n');

    code(text, text.length(), seed);
    save_text("../DesafioEvaluativoV2/data/sales_record.txt", text, text.length());
}

void reset_sales_record(array<unsigned int, 6> &sales, unsigned long long int &total, unsigned int seed) {
    total = 0;
    for (short int i = 0; i < 6; i++) sales.at(i) = 0;
    save_sales(sales, total, seed);
}

//Precios:

//-----3D
//11700
//13900
//22900

//-----2D
//8700
//10900
//19900

void display_sales(const array<unsigned int, 6> &sales, const unsigned long long int &total) {

    system("cls");
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    SetConsoleTextAttribute(hConsole, 2);
    cout << endl << "           TODAY'S SALES" << endl << endl;
    SetConsoleTextAttribute(hConsole, 7);

    cout << "  -----------------------------3D" << endl << endl;

    cout << "  General: " << sales.at(0) << "  ---  $" << sales.at(0)*11700 << endl << endl;
    cout << "  VibroSound: " << sales.at(1) << "  ---  $" << sales.at(1)*13900 << endl << endl;
    cout << "  Gold: " << sales.at(2) << "  ---  $" << sales.at(2)*22900 << endl << endl;

    cout << "  -----------------------------2D" << endl << endl;

    cout << "  General: " << sales.at(3) << "  ---  $" << sales.at(3)*8700 << endl << endl;
    cout << "  VibroSound: " << sales.at(4) << "  ---  $" << sales.at(4)*10900 << endl << endl;
    cout << "  Gold: " << sales.at(5) << "  ---  $" << sales.at(5)*19900 << endl << endl;

    cout << "  --------------------Total: $" << total << endl << endl;

    cout << "  There is also a more compact version of the sale" << endl;
    cout << "  record in sales_record.txt, but it is encoded" << endl << endl << "  ";

    system("pause");
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





















