#include <iostream>
#include <fstream>
#include "code_decode.h"

using namespace std;

bool str2int(string str, unsigned int &num) {
    //Retorna True si el string es un entero no negativo.
    //En dicho caso devuelve el entero en num.

    num = 0;
    unsigned int len = unsigned(str.length());
    if (len == 0) return false;
    for (unsigned int i = 0; i < len; i++) {
        if (isdigit(str[i])) {
            num = 10*num + unsigned(int(str[i])) - 48;
        }
        else return false;
    }
    return true;
}

//Overloaded Function
bool str2int(string str, unsigned long long int &num) {

    num = 0;
    unsigned int len = unsigned(str.length());
    if (len == 0) return false;
    for (unsigned int i = 0; i < len; i++) {
        if (isdigit(str[i])) {
            num = 10*num + unsigned(int(str[i])) - 48;
        }
        else return false;
    }
    return true;
}

bool get_valid_input(unsigned int &num) {
    //Pregunta por el valor de un entero positivo.

    string ans;
    fflush(stdin);
    getline(cin, ans);
    return str2int(ans, num);
}

//Overloaded Function
bool get_valid_input(unsigned long long int &num) {

    string ans;
    fflush(stdin);
    getline(cin, ans);
    return str2int(ans, num);
}

unsigned int ask(string question, string *&options, unsigned short int num) {
    //Recibe un string con una pregunta que muestra en pantalla,
    //un array de strings que son opciones que también son impresas en pantalla y
    //el número de éstas.

    unsigned int opt;
    string ans;
    cout << question << endl << endl;
    for (short int i = 0; i < num; i++) cout << (i + 1) << ". " << options[i] << endl;

    cout << endl << "Enter your option (1 - " << num << "): ";
    while (!get_valid_input(opt) or (opt < 1) or (num < opt)) {
        cout << "Sorry, that is not a valid option" << endl << endl;
        cout << "Enter your option (1 - " << num << "): ";
    }
    delete[] options;
    return opt;
}

void get_file_name(char *&file_name, bool txt) {
    //Pregunta al usuario por el nombre de algún archivo utilizando char*.
    //Lo devuelve en file_name y le concatena .txt o .dat dependiendo de si
    //txt es True o False.

    char ans[51];
    unsigned int len = 0, i = 0;

    fflush(stdin);
    cin.getline(ans, 51, '\n');
    while (ans[len] != '\0') len++;

    file_name = new char[len + 5];
    file_name[len + 4] = '\0';
    for (; i < len; i++) file_name[i] = ans[i];
    if (txt) {
        file_name[i++] = '.';
        file_name[i++] = 't';
        file_name[i++] = 'x';
        file_name[i] = 't';
    }
    else {
        file_name[i++] = '.';
        file_name[i++] = 'd';
        file_name[i++] = 'a';
        file_name[i] = 't';
    }
}

bool Qfile(char file_name[]) {
    //Verifica que el archivo exista.
    fstream file(file_name, ios::in);
    if (file.is_open()) {
        file.close();
        return true;
    }
    else return false;
}

//Overloaded Funcion
bool Qfile(string file_name) {

    fstream file(file_name, ios::in);
    if (file.is_open()) {
        file.close();
        return true;
    }
    else return false;
}

void get_files_names(string &source_file, string &output_file, bool code) {

    fflush(stdin);
    cout << "Enter the file name to " << (code?"encode":"decode") <<" (without the " << (code?".txt":".dat") << "): "; getline(cin, source_file);
    source_file += (code?".txt":".dat");
    while (!Qfile(source_file)) {
        cout << "Sorry, the file " << source_file << " could not be opened" << endl << endl;
        fflush(stdin);
        cout << "Enter the file name to " << (code?"encode":"decode") << " (without the " << (code?".txt":".dat") << "): "; getline(cin, source_file);
        source_file += (code?".txt":".dat");
    }

    fflush(stdin);
    cout << "Enter the file name where " << (code?"encoding":"decoding") << " will go (without the " << (code?".dat":".txt") << "): "; getline(cin, output_file);
    output_file += (code?".dat":".txt");
}

unsigned int get_seed() {
    //Pregunta por la semilla de codificación.

    unsigned int seed;
    cout << "\nEnter the codification seed: ";
    while (!get_valid_input(seed) or (seed == 0)) {
        cout << "Sorry, the seed must be a positive integer" << endl << endl;
        cout << "Enter the codification seed: ";
    }
    return seed;
}

void codification_methodsSTR(bool code) {
    //Métodos de codificación utilizando strings.
    //Si bool es True se codifica, en caso contrario se decodifica.
    //Se parece mucho a la función anterior pues la mayoria de funciones
    //utilizadas dentro están sobrecargadas, por lo cual al llamarlas
    //con strings o char* se define cuál utilizar.

    bool method;
    unsigned int seed;
    string source_file, output_file;

    cout << endl;
    get_files_names(source_file, output_file, code);

    string *options = new string[2]{"Method 1", "Method 2"};
    if (ask("\nWhich method do you want to use?", options, 2) == 1) method = 0;
    else method = 1;

    seed = get_seed();

    if (code) {
        system("cls");
        if (code_file(source_file, output_file, seed)) cout << "The codification was successful!" << endl << endl;
        else cout << "The codification was NOT successful, there was an error" << endl << endl;
    }
    else {
        system("cls");
        if (decode_file(source_file, output_file, seed)) cout << "The decodification was successful!" << endl << endl;
        else cout << "The decodification was NOT successful, there was an error" << endl << endl;
    }

    cout << (code?"Normal":"Code") << " File: " << source_file << endl;
    cout << (code?"Code":"Normal") << " File: " << output_file << endl;
    cout << "Method: " << (method + 1) << endl;
    cout << "Seed: " << seed << endl << endl;
    system("pause");
}

unsigned int display_codification_menu() {
    //Muestra el menú de codificación.
    system("cls");
    cout << endl << "-----------------CODIFICATION METHODS-----------------" << endl << endl;
    string *options = new string[5]{"Codification with char*", "Codification with strings", "Decodification with char*", "Decodification with strings", "Exit"};
    return ask("Do you want to use char* or strings?, Encode or Decode?", options, 5);
}

//void codification_methods() {
//    //Administra el menú de los métdodos de codificación.

//    unsigned int opt = display_codification_menu();
//    while (opt != 5) {
//        if (opt == 1) codification_methodsCHAR(true);
//        else if (opt == 2) codification_methodsSTR(true);
//        else if (opt == 3) codification_methodsCHAR(false);
//        else if (opt == 4) codification_methodsSTR(false);
//        opt = display_codification_menu();
//    }
//    system("cls");
//}

unsigned int display_main_menu() {
    //Muestra el menú principal de la aplicación.
    system("cls");
    cout << endl << "----------------------WELCOME TO THE ATM----------------------" << endl << endl;
    string *options = new string[3]{"Login as admin", "Login as an user", "Exit"};
    return ask("What do you want to do?", options, 3);
}

unsigned int display_user_menu() {
    //Muestra el menú principal de la sesión de usuario.
    cout << endl << "-------------------------USER SESSION-------------------------" << endl << endl;
    string *options = new string[3]{"Check the balance", "Withdraw money", "Exit"};
    return ask("What do you want to do?", options, 3);
}

unsigned int display_practice_menu() {
    //Muestra el menú de toda la práctica.
    system("cls");
    string *options = new string[3]{"Codification methods", "ATM application", "Exit"};
    return ask("     .: Practice 3 - Tobias Garcia Mejia :.", options, 3);
}
