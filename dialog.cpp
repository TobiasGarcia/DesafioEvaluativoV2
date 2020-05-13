#include "dialog.h"
#include <iostream>
#include <fstream>
#include "code_decode.h"

using namespace std;

string get_non_empty_line(string question, string warning) {

    string ans;
    bool repeat = true;

    //Este ciclo se pudo haber hecho con un while (true), pero en lo personal prefiero no utilzarlo
    //pues no me parece una buena práctica de programación.

    while (repeat) {
        cout << "\n  " << question << ' ';
        fflush(stdin);
        getline(cin, ans);
        if (ans.length() != 0) repeat = false;
        else cout << warning << endl;
    }
    return ans;
}

bool yes_no_question(string question, string warning) {

    string ans;
    short int len;
    bool repeat = true, bool_ans;

    //Este ciclo se pudo haber hecho con un while (true), pero en lo personal prefiero no utilzarlo
    //pues no me parece una buena práctica de programación.

    while (repeat) {
        cout << "\n  " << question << ' ';
        fflush(stdin);
        getline(cin, ans);

        len = ans.length();
        if ((len == 3) and ((ans[0] == 'y') or (ans[0] == 'Y')) and ((ans[1] == 'e') or (ans[1] == 'E')) and ((ans[2] == 's') or (ans[2] == 'S'))) {
            repeat = false;
            bool_ans = true;
        }
        else if ((len == 2) and ((ans[0] == 'n') or (ans[0] == 'N')) and ((ans[1] == 'o') or (ans[1] == 'O'))) {
            repeat = false;
            bool_ans = false;
        }
        else cout << warning << endl;
    }
    return bool_ans;
}

bool get_id_input(unsigned long long int &id, bool &is_admin) {

    //Solicitamos al usuario una cédula, las cuales están compuestas
    //únicamente por enteros desde el 0 hasta el 9. Retornamos true
    //y almacenamos la cédula ingresada en id en caso de que
    //efectivamente sólo estuviera compuesta por enteros desde
    //el 0 hasta el 9, o en el caso de que se haya ingresado
    //la palabra clave 'admin', retornamos de nuevo true pero
    //no modificamos la variable id y colocariamos is_admin en
    //true. Retornamos false si no se presenta alguno de los
    //dos casos anteriormente mencionados.

    string ans;
    is_admin = false;

    fflush(stdin);
    getline(cin, ans);

    if (ans == "admin") {
        is_admin = true;
        return true;
    }
    else return str2int(ans, id);
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

short int ask(string question, string *&options, short int num) {
    //Recibe un string con una pregunta que muestra en pantalla,
    //un array de strings que son opciones que también son impresas en pantalla y
    //el número de éstas.

    string ans;
    short int opt;

    cout << question << endl << endl;
    for (short int i = 0; i < num; i++) cout << (i + 1) << ". " << options[i];

    opt = get_int_input("Enter your option (1 - " + to_string(num) + "): ", "Sorry, that is not a valid option", short(1), num);
    delete[] options;
    return opt;
}

void display_welcome() {
    system("cls");
    cout << endl << " WELCOME TO THE CINEMA!";
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

short int get_seed() {
    //Pregunta por la semilla de codificación.

    short int seed;
    seed = get_int_input("Enter the codification seed: ", "Sorry, the seed must be a positive integer", short(1));
    return seed;
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
