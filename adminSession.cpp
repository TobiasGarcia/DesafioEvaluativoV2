#include "adminSession.h"
#include "code_decode.h"
#include "dialog.h"
#include "login.h"
#include <windows.h>

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

void admin_session(vector<Show> &shows, const unsigned int &code_seed, bool &on) {

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
        else if (opt == 3) cout << "SALES REPORT";
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
    cout << endl << "  You choose the " << combo << " combo!" << endl << endl;
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

void user_session(vector<Show> &shows, const unsigned long long int &user_id, const unsigned int &seed) {

    //Esta función es muy pequeña pero es para no sobrecargar mucho el main y mantenerlo ordenado.

    short int id;
    if (get_show_id(shows, id, false)) {
        shows.at(id).reserve_seat(shows.size(), user_id, seed);
    }
}
