#include "dialog.h"
#include <array>
#include <fstream>
#include <iostream>
#include <windows.h>

using namespace std;

//Esta libreria contiene la mayor parte de funciones que validan entradas del usuario,
//o imprimen algo en pantalla.

//Nota: Hay 3 funciones plantilla en dialog.h que nos ayudan a hacer la validación,
//is_in_range(), str2int() y get_int_input().


//------------------------------------------------------------------------------------------------
//---------------------------FUNCIONES PARA VALIDACIÓN DE INPUTS----------------------------------
//------------------------------------------------------------------------------------------------

string get_non_empty_line(string question, string warning) {

    //El string question es la pregunta que se le hará al usuario,
    //mientras que warning es la advertencia que se le dará en caso
    //de ingresar una línea vacía.

    string ans;
    bool ask = true;

    //Este ciclo se pudo haber hecho con un while (true), pero en lo personal prefiero no utilzarlo
    //pues no me parece una buena práctica de programación.

    while (ask) {

        cout << "\n  " << question << ' ';

        fflush(stdin);
        getline(cin, ans);

        if (ans.length() != 0) ask = false;
        else cout << warning << endl;
    }
    return ans;
}

bool yes_no_question(string question, string warning) {

    //El string question es la pregunta que se le hará al usuario,
    //mientras que warning es la advertencia que se le dará en caso
    //de ingresar algo diferente a un 'Yes' o 'No'.

    //Nota: Esta función es independiente de las mayúsculas o minúsculas (no es case sensitive),
    //es decir, reconoce 'yes', 'YES', 'Yes', etc. como una respuesta afirmativa,
    //análogamente con 'no', 'NO', 'No', etc.

    string ans;
    short int len;
    bool bool_ans, ask = true;

    //Este ciclo se pudo haber hecho con un while (true), pero en lo personal prefiero no utilzarlo
    //pues no me parece una buena práctica de programación.

    while (ask) {
        cout << "\n  " << question << ' ';

        fflush(stdin);
        getline(cin, ans);

        len = ans.length();

        //Estos condicionales son para hacer que la función no sea case sensitive.

        if ((len == 3) and ((ans[0] == 'y') or (ans[0] == 'Y')) and ((ans[1] == 'e') or (ans[1] == 'E')) and ((ans[2] == 's') or (ans[2] == 'S'))) {
            ask = false;
            bool_ans = true;
        }
        else if ((len == 2) and ((ans[0] == 'n') or (ans[0] == 'N')) and ((ans[1] == 'o') or (ans[1] == 'O'))) {
            ask = false;
            bool_ans = false;
        }
        else cout << warning << endl;
    }
    return bool_ans;
}

bool get_id_input(unsigned long long int &id, bool &is_admin) {

    //Solicitamos al usuario una cédula, las cuales están compuestas únicamente por enteros desde
    //el 0 hasta el 9. Retornamos true y almacenamos la cédula ingresada en id en caso de que
    //efectivamente sólo estuviera compuesta por enteros desde el 0 hasta el 9; o en el caso
    //de que se haya ingresado la palabra clave 'admin', retornamos de nuevo true pero no
    //modificamos la variable id y colocariamos is_admin en true. Retornamos false si
    //no se presenta alguno de los dos casos anteriormente mencionados.

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

bool exists_file(string file_name) {

    fstream file("../DesafioEvaluativoV2/data/" + file_name, ios::in);

    if (file.is_open()) {
        file.close();
        return true;
    }
    else return false;
}

void get_files_names(string &source_file, string &output_file, bool code) {

    //Solicitamos al usuario el nombre de dos archivos para codifciar o decodificar
    //la información del primero y almacenarla en el segundo, el primero es el texto
    //fuente, el segundo es donde se guardará el texto codificado o decodificado
    //dependiendo de si code es true o false respectivamente.

    fflush(stdin);
    cout << endl << "  Enter the file name to " << (code?"encode":"decode") <<" (without the .txt): "; getline(cin, source_file);

    //Le agregamos la extensiñon .txt desde dentro del código para facilidad del usuario.

    source_file += ".txt";
    while (!exists_file(source_file)) {

        cout << "  Sorry, the file " << source_file << " could not be opened" << endl << endl;
        fflush(stdin);

        cout << "  Enter the file name to " << (code?"encode":"decode") << " (without the .txt): "; getline(cin, source_file);
        source_file += ".txt";
    }

    //Notemos que aquí no hay que validar si el archivo existe o no, porque al ser el
    //archivo de salida, en caso de que éste no exista se creará.

    fflush(stdin);
    cout << "  Enter the file name where " << (code?"encoding":"decoding") << " will go (without the .txt): "; getline(cin, output_file);
    output_file += ".txt";
}

bool get_show_id(const vector<Show> &shows, short int &id, bool is_admin) {

    //Imprimimos en pantalla los shows del vector shows y le solicitamos al
    //usuario que ingrese la ID de algúno que sea de su interés; cuando
    //ingrese una ID válida está será almacenada en la variable id
    //recibida por referencia. La variable is_admin es true cuando
    //es el admin quien invoca ésta función; la diferencia radica
    //simplemente en los mensajes que se le muestran al usuario.
    //Retornamos true si el usuario elige una película, o false en
    //caso de que prefiera cancelar la elección.

    string msg;
    bool ask = true;
    short int shows_num = shows.size();

    if (is_admin) msg = "Enter the ID of the show where you want to offer seats, or 0 for exit:";
    else msg = "Enter the ID of the show where you want to reserve a seat, or 0 for exit:";

    //Este ciclo se pudo haber hecho con un while (true), pero en lo personal prefiero no utilzarlo
    //pues no me parece una buena práctica de programación.

    while (ask) {

        display_shows(shows);

        //Le restamos 1 al retorno de get_int_input() para pasar de índices de la forma 1, 2, 3, ... (usuario)
        //a su correspondiente en la forma 0, 1, 2, ... (programador).

        id = get_int_input(msg, "Sorry, the ID must be a number between 1 and " + to_string(shows_num), short(0), shows_num) - 1;

        //Ofrecemos la posibilidad de cancelar la elección de la película si se ingresa 0,
        //en cuyo caso id será -1 y por tanto retornamos false.

        if (id == -1) return false;

        //Verificamos que la sala no esté llena. Notemos que esto se debe hacer tanto en la
        //versión de usuario como en la de administrador, pues el administrador no podrá
        //modificar el modo en que se ofrece ningún asiento ya que todos estarían vendidos.

        else if (shows.at(id).get_empty_places() == 0) {
            cout << endl << "  Sorry, this show is full" << endl;
            ask = yes_no_question("Do you want to select other show? (Enter 'Yes' to select it, or 'No' to exit)");
            system("cls");
        }
        else return true;
    }
    return false;
}

bool charge_money(unsigned int price) {

    //Cobramos el dinero ingresado en la variable price. Notemos que basta con un unsigned int
    //pues el programa reserva los asientos uno por uno, y el precio más alto que un asiento
    //puede alcanzar es de $22900. Retorna true en caso de que sí se realice la transacción,
    //o false en caso de que el usuario decida cancelar la reserva.

    string ans;
    bool ask = true;
    unsigned int money, num_bills, bills[] = {50000, 20000, 10000, 5000, 2000, 1000, 500, 200, 100, 50};

    //Reutilizamos el código del problema 1 de la práctica 2, pero le agregamos la validación
    //de la entrada y otros detalles para mejorarlo a como estaba, y adapatarlo a éste escenario.
    //Más que todo cambiar los mensajes de diálogo, pero el resto es idéntico.

    while (ask) {
        cout << endl << "  Enter the money please: (or 0 for cancel the reserve) ";

        fflush(stdin);
        getline(cin, ans);

        //Dejamos abierta la posibilidad al usuario de cancelar la transacción si
        //ingresa 0, uno nunca sabe si puede que no le alcanzase el dinero, y
        //naturalmente no podemos dejar bloqueada la aplicación en éste escenario.

        if (!str2int(ans, money)) cout << "  Sorry, you only can enter numbers" << endl;
        else if (money == 0) return false;
        else if (money < price) cout << "  Sorry, you must enter a minimum amount of $" << price << endl;
        else ask = false;
    }

    system("cls");

    //Honestamente las líneas del HANDLE y SetConsoleTextAttribute() las tuve que copiar
    //y pegar de internet, más especificamente de éste foro:

    //https://stackoverflow.com/questions/4053837/colorizing-text-in-the-console-with-c

    //Se utilizan para poder manejar colores en consola.

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

short int offer_combos() {

    //Le ofrecemos los combos al usuario que haya reservado un asiento Gold.
    //Recordemos que esto es para darle una diferencia al asiento Gold de los demás,
    //en un escenario real esta parte del código debería integrarse con el sistema
    //para administrar la venta de alimentos, pero como se menciona en la explicación
    //de los tipos de asientos dentro del vídeo de Youtube, esta parte es provisional.
    //Retorna el número del combo seleccionado 1, 2, 3 o 4.

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


//------------------------------------------------------------------------------------------------
//---------------------------FUNCIONES PARA IMPRIMIR EN PANTALLA----------------------------------
//------------------------------------------------------------------------------------------------

void msg_and_cls(string msg) {

    //Esta función es muy corta pero se repetía en muchas partes del código,
    //por lo cual la decidí implementar para poder reutilizarla y que el código
    //se viera más ordenado.

    cout << "  " << msg << endl << endl << "  ";
    system("pause");
    system("cls");
}

short int explain_offer_types(const Seat &seat, const short int &row, const short int &column) {

    //Recibimos una estructura asiento llamada seat, revisamos en que modalidad es ofrecida al
    //público, y dependiendo de ésta, le explicamos al usuario en que consiste y cuanto
    //cuesta reservar ese tipo de asiento. No solicitamos ningún input, solo es una
    //función para imprimir la explicación. Aprovechamos también para retornar
    //el precio del tipo de asiento seleccionado.

    //Honestamente las líneas del HANDLE y SetConsoleTextAttribute() las tuve que copiar
    //y pegar de internet, más especificamente de éste foro:

    //https://stackoverflow.com/questions/4053837/colorizing-text-in-the-console-with-c

    //Se utilizan para poder manejar colores en consola.

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

void display_title(bool is_admin) {

    //Limpiamos la pantalla e imprimimos en ella un mensaje de cabecera;
    //si es para el admin ponemos el título para el menú de administrador,
    //pero si es para el usuario imprimimos un mensaje de bienvenida al cine.

    system("cls");
    if (is_admin) {
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

        SetConsoleTextAttribute(hConsole, 6);
        cout << endl << "      .: ADMINISTRATION MENU :.";
        SetConsoleTextAttribute(hConsole, 7);
    }
    else {
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

        SetConsoleTextAttribute(hConsole, 3);
        cout << endl << "         .: WELCOME TO THE CINEMA :." << endl;
        SetConsoleTextAttribute(hConsole, 7);
    }
}

void display_sales(const array<unsigned int, 6> &sales, const unsigned long long int &total) {

    //Imprimimos en pantalla el registro ventas almacenado en el array sales.

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

void display_edge(short int size, short int chr) {

    //Imprimimos en pantalla un borde de tamaño size y conformado
    //por los carácteres chr. Esta función es con el propósito
    //de reutilizar el código.

    for (short int i = 0; i < size; i++) cout << char(205);
    cout << char(chr);
}

void display_adapter_separator(short int chr1, short int chr2, short int chr3, short int size1, short int size2) {

    //Imprimimos en pantalla un separador para una matriz. Cada columna de la matriz
    //posee una longitud específica. Esta función es utilizada para imprimir los
    //bordes de la cartelera de películas.

    //Nota: En general las funciones se deberían implementar siempre pensando
    //en la portabilidad de éstas, pero lamentablemente esta función es demasiado
    //específica para este problema, así como la mayor parte de las otras funciones
    //para imprimir en pantalla, pues la forma en que se imprimen los datos en la
    //consola es algo muy característico de cada quien, y es incluso común que
    //cambie dependiendo del proyecto.

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

    //Imprime en pantalla el string data con una longitud de size
    //espacios, pero colocando data en el centro, lo que sobra es
    //rellenado con ' ' (espacio vacío).

    //Nota: Preferí pasar todos los datos en tipo string en lugar
    //de hacer de ésta función una plantilla, pues es mucho más
    //cómodo al momento de mediar la longitud de los datos que
    //serán impresos en pantalla.

    //Añadimos también dos espacios vaciós en los extremos de data, sólo es por
    //gusto personal, por eso es que se resta 2.

    short int spaces = (size - data.length() - 2), aux = int(spaces/2);

    for (short int i = 0; i < aux; i++) cout << ' ';
    cout << ' ' << data << ' ';
    for (short int i = 0; i < (spaces - aux); i++) cout << ' ';
}

void display_separator(short int size, short int chr1, short int chr2, short int chr3) {

    //Imprime en pantalla un separador para una matriz cuyas columnas tienen un tamaño
    //fijo. De nuevo es una funcion cuyo propósito es reutilizar código.

    cout << char(chr1);
    for (int i = 0; i < (size - 1); i++) cout << char(196) << char(196) << char(196) << char(chr2);
    cout << char(196) << char(196) << char(196) << char(chr3);
}

void colored_display(const Seat &seat, const bool &is_admin) {

    //Imprimimos en pantalla el asiento seat teniendo en cuenta
    //el color según la forma en que se ofrece al público,
    //y la forma dependiendo de si está ocupado o no.

    //Colores: Blanco para General, rojo para VibroSound y amarillo para Gold.
    //Formas: O para asiento vacío, X para asiento ocupado.

    //Nota: Como se menciona en la explicación de de los tipos de asientos dentro
    //del vídeo de Youtube, la forma extra ■ (el char(254)) está reservada para que
    //el administrador pueda identificar cuales asientos cuentan con la tecnología
    //VibroSound.

    char chr;
    if (!seat.is_empty) chr = 'X';
    else if (is_admin and seat.is_vibro) chr = char(254); //Éste carácter es el ■.
    else chr = 'O';

    //Recordemos que los enteros diferentes de 0 siempre arrojan true al ser juzgados
    //dentro de un condicional, luego el if() se cumplirá si el asiento se oferta
    //en 1 (VibroSound) o 2 (Gold), por lo cual le debemos cambiar el color.

    if (seat.sale_type) {

        //Honestamente las líneas del HANDLE y SetConsoleTextAttribute() las tuve que copiar
        //y pegar de internet, más especificamente de éste foro:

        //https://stackoverflow.com/questions/4053837/colorizing-text-in-the-console-with-c

        //Se utilizan para poder manejar colores en consola.

        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

        SetConsoleTextAttribute(hConsole, (seat.sale_type == 2)?6:12);
        cout << chr;
        SetConsoleTextAttribute(hConsole, 7);
    }
    else cout << chr;
}

void display_wall(short int left, short int walls, short int right) {

    //Imprime una de las paredes conformadas por ▒ (el char(177)) de la sala de cine.
    //De nuevo esta función es sólo con el propósito de reutilizar código.

    for (short int j = 0; j < left; j++) cout << ' ';
    for (short int j = 0; j < walls; j++) cout << char(177);
    for (short int j = 0; j < right; j++) cout << ' ';
}
