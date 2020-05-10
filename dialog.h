#ifndef DIALOG_H
#define DIALOG_H

#include <iostream>

using namespace std;

void display_welcome();

bool get_id_input(unsigned long long int &num, bool &is_admin);

unsigned int ask(string question, string *&options, unsigned short int num);

void get_file_name(char *&file_name, bool txt);

bool Qfile(char file_name[]);

//Overloaded Funcion
bool Qfile(string file_name);

void get_files_names(string &source_file, string &output_file, bool code);

unsigned int get_seed();

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

#endif // DIALOG_H
