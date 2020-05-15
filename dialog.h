#ifndef DIALOG_H
#define DIALOG_H

#include <iostream>
#include "show.h"

using namespace std;


//-------------------------------------------------------------FUNCIONES PARA VALIDACIÓN DE INPUTS

string get_non_empty_line(string question, string warning = "  Sorry, it can't be an empty space.");

bool yes_no_question(string question, string warning = "  Sorry, the answer can only be 'Yes' or 'No'");

bool get_id_input(unsigned long long int &id, bool &is_admin);

bool exists_file(string file_name);

void get_files_names(string &source_file, string &output_file, bool code);

bool get_show_id(const vector<Show> &shows, short int &id, bool is_admin);

bool charge_money(unsigned int price);

short int offer_combos();


//-------------------------------------------------------------FUNCIONES PARA IMPRIMIR EN PANTALLA

void msg_and_cls(string msg);

short int explain_offer_types(const Seat &seat, const short int &row, const short int &column);

void display_title(bool is_admin);

void display_sales(const array<unsigned int, 6> &sales, const unsigned long long int &total);

void display_edge(short int size, short int chr);

void display_adapter_separator(short int chr1, short int chr2, short int chr3, short int size1, short int size2);

void centred_display(string data, const short int &size);

void display_separator(short int size, short int chr1, short int chr2, short int chr3);

void colored_display(const Seat &seat, const bool &is_admin);

void display_wall(short int left, short int walls, short int right);


//-----------------------------------------------------------------------------FUNCIONES PLANTILLA

//Retorna true si el número num está entre min y max, inclusive.
template <typename T>
bool is_in_range(T num, short int min, short int max) {return ((min <= short(num)) and (short(num) <= max));}

template <typename T>
bool str2int(string str, T &num) {

    //Retorna true si el string str está conformado exclusivamente por digitos
    //entre el 0 y el 9, inclusive, y en éste caso el número entero al que
    //corresponde es almacenado en la variable num recibida por referencia.
    //Retorna false en otro caso.

    num = 0;
    unsigned int len = str.length();
    if (len == 0) return false;

    for (unsigned int i = 0; i < len; i++) {

        //Si no es un digito retornamos false.

        if (is_in_range(str[i], 48, 57)) num = 10*num + (str[i] - 48);
        else return false;
    }
    return true;
}

template <typename T>
T get_int_input(string question, string warning, T min, T max = 0) {

    //Muestra al usuario la pregunta question y le solicita ingresar un entero no negativo,
    //si éste número está entre min y max, inclusive, es retornado por la función, en caso
    //contrario o en el caso de que el input no sea un entero no negativo, imprimirá
    //warning y solicitará otro input.

    //Si no se requiere mínimo, se coloca 0, pues es el menor de los enteros no negativos,
    //y si no se requiere máximo se puede omitir o colocar en 0, en cuyo caso la función
    //interpretará que no se requiere validación para una cota superior.

    //Nota: Si se piensa con detenimiento notaremos que necesitar que el máximo sea 0
    //es un redundante lógico cuando se trabaja con enteros no negativos, por lo
    //cual lo podemos usar para determinar cuando no se necesita máximo sin temor
    //a limitar la función.

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
