#ifndef DIALOG_H
#define DIALOG_H

#include <iostream>

using namespace std;

string get_non_empty_line(string question, string warning = "  Sorry, it can't be an empty space.");

bool yes_no_question(string question, string warning = "  Sorry, the answer can only be 'Yes' or 'No'");

bool get_id_input(unsigned long long int &num, bool &is_admin);

void get_file_name(char *&file_name, bool txt);

short int ask(string question, string *&options, short int num);

void display_welcome();

bool Qfile(char file_name[]);

//Overloaded Funcion
bool Qfile(string file_name);

void get_files_names(string &source_file, string &output_file, bool code);

short int get_seed();

void codification_methodsSTR(bool code);

unsigned int display_codification_menu();

unsigned int display_main_menu();

unsigned int display_user_menu();

unsigned int display_practice_menu();

template <typename T>
bool str2int(string str, T &num) {

    num = 0;
    unsigned int len = str.length();
    if (len == 0) return false;

    for (unsigned int i = 0; i < len; i++) {

        //Si no es un digito retornamos false.

        if ((str[i] < 48) or (57 < str[i])) return false;
        else num = 10*num + (str[i] - 48);
    }
    return true;
}

template <typename T>
T get_int_input(string question, string warning, T min, T max = 0) {

    //Si no se requiere mínimo, se coloca 0, pues es el menor de los enteros no negativos.

    //No tiene sentido solicitar sólo el número 0, pues sólo continuará la función
    //cuando la entrada sea un 0, por lo cual de antemano sabiamos que la entrada
    //sería un 0, entonces no hacía falta solicitar un dato.

    T num;
    string ans;

    cout << "\n  " << question << ' ';
    fflush(stdin);
    getline(cin, ans);
    while (!str2int(ans, num) or (num < min) or ((max == 0)? false:(max < num))) {
        cout << "  " << warning << endl << endl;
        cout << "  " << question << ' ';
        fflush(stdin);
        getline(cin, ans);
    }
    return num;
}

#endif // DIALOG_H
