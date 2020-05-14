#include "adminSession.h"
#include "code_decode.h"
#include "dialog.h"
#include "login.h"
#include <windows.h>
#include <array>
#include <fstream>

void display_admin_menu() {
    display_welcome(true);
    cout << endl << endl << "  What you want to do?" << endl;
    cout << endl << "  1. Schedule new show" << endl;
    cout << "  2. Offer seats at scheduled shows" << endl;
    cout << "  3. Make sales report of today" << endl;
    cout << "  4. Encode file" << endl;
    cout << "  5. Decode file" << endl;
    cout << "  6. Turn off the application" << endl;
    cout << "  7. Exit" << endl;
}

void admin_session(vector<Show> &shows, const array<unsigned int, 6> &sales, const unsigned long long int total, const unsigned int &code_seed, bool &on) {

    short int opt = 1, id;

    while (opt != 7) {

        display_admin_menu();
        opt = get_int_input("What you want to do?", "Sorry, that is not a valid option", short(1), short(7));

        //Siempre he preferido el if en lugar de switch.

        if (opt == 1) {
            system("cls");
            add_show(shows);
        }
        else if (opt == 2) {
            system("cls");
            if (get_show_id(shows, id, true)) shows.at(id).offer_seats(shows.size());
        }
        else if (opt == 3) {
            display_sales(sales, total);
        }
        else if ((opt == 4) and login_admin(code_seed)) {
            system("cls");
            string file_nat, file_code;
            get_files_names(file_nat, file_code, true);
            code_file("../DesafioEvaluativoV2/data/" + file_nat, "../DesafioEvaluativoV2/data/" + file_code, code_seed);
        }
        else if ((opt == 5)  and login_admin(code_seed)) {
            system("cls");
            string file_nat, file_code;
            get_files_names(file_code, file_nat, false);
            decode_file("../DesafioEvaluativoV2/data/" + file_code, "../DesafioEvaluativoV2/data/" + file_nat, code_seed);
        }
        else if (opt == 6) {
            on = false;
            opt = 7;
        }
    }
}

short int offer_combos() {

    short int combo;
    cout << endl <<  "  You reserved one Gold seat, so you can choose one of these combos*" << endl;
    cout << "  and it will be brought to your seat before the movie: " << endl << endl;
    cout << "  1. Large popcorn with medium soda or tea" << endl;
    cout << "  2. Hot dog with medium soda or tea" << endl;
    cout << "  3. Nachos with guacamole and sour cream" << endl;
    cout << "  4. Super burger with small soda" << endl;
    cout << endl << "  *All offers are subject to change and availability" << endl;
    combo = get_int_input("Which one will you take? (1 - 4)", "Sorry, only can be one of these four combos", 1, 4);
    cout << endl << "  You choose the combo " << combo << '!' << endl << endl << "  ";
    system("pause");
    return combo;
}

bool charge_money(unsigned int price) {

    string ans;
    bool ask = true;
    unsigned int money, num_bills, bills[] = {50000, 20000, 10000, 5000, 2000, 1000, 500, 200, 100, 50};

    while (ask) {
        cout << endl << "  Enter the money please: (or 0 for cancel the reserve) ";
        fflush(stdin);
        getline(cin, ans);

        if (!str2int(ans, money)) cout << "  Sorry, you only can enter numbers" << endl;
        else if (money == 0) return false;
        else if (money < price) cout << "  Sorry, you must enter a minimum amount of $" << price << endl;
        else ask = false;
    }

    system("cls");
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    SetConsoleTextAttribute(hConsole, 6);
    cout << endl << "  Your reservation has been successful!" << endl;
    SetConsoleTextAttribute(hConsole, 7);

    money -= price;
    if (money >= 50) {
        cout << endl << "  Here is your change: $" << money << endl << endl;
        for (short int i = 0; i < 10; i++) {

            num_bills = unsigned(int(money/bills[i]));
            money %= bills[i];

            if ((num_bills != 0) and (i < 5)) cout << "  " << num_bills << ((num_bills>1)?" bills":" bill") << " of $" << bills[i] << endl;
            else if (num_bills != 0) cout << "  " << num_bills << ((num_bills>1)?" coins":" coin") << " of $" << bills[i] << endl;
        }
    }

    if (money != 0) {
        cout << endl << "  Sorry, but there is a remainder of $" << money << " that we can't" << endl;
        cout << "  give you because the smallest coin is $50" << endl;
    }
    cout << endl << "  ";
    system("pause");
    return true;
}

void user_session(vector<Show> &shows, array<unsigned int, 6> &sales, unsigned long long int &total, const unsigned long long int &user_id, const unsigned int &seed) {

    //Esta función es muy pequeña pero es para no sobrecargar mucho el main y mantenerlo ordenado.

    short int id;
    if (get_show_id(shows, id, false)) {
        shows.at(id).reserve_seat(shows.size(), sales, total, user_id, seed);
    }
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























