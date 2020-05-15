#include "show.h"
#include <vector>
#include <fstream>
#include <windows.h>
#include "session.h"
#include "dialog.h"
#include "login.h"

//La clase Show está pensada para modelar las películas que serán ofrecidas en la cartelera del cine.


//------------------------------------------------------------------------------------------
//------------------------MÉTODOS PARA MANEJAR LOS ARCHIVOS TXT-----------------------------
//------------------------------------------------------------------------------------------

void Show::fill_row(string line, short row) {

    //Recibe una fila de la matriz de asientos en su formato de string y la
    //interpreta para llenar la matriz de asientos seats[][] con esa información.

    //Recordemos que como se explica en el vídeo de Youtube, una fila en su forma de
    //string puede verse como sigue:

    //10 10 11 11 02 12 11 10 10 10

    //Donde cada cada par de números representa un asiento. El primero indica si el
    //asiento está vacío, mientras que el segundo indica en que modo se está ofreciendo
    //el asiento al público, 0 para General, 1 para VibroSound y 2 para Gold.

    //Dependiendo de la fila pueden haber espacios en blanco antes, pues la sala de cine
    //entera en su forma de string se ve así:

    //10 10 10 10 10 10 10 10 10 10
    //10 10 10 10 00 00 00 10 10 10
    //10 10 11 11 02 12 11 10 10 10
    //10 10 01 02 02 02 02 11 10 10
    //   10 10 11 02 12 11 10 10
    //      10 10 10 00 00 10
    //         10 10 10 10

    //Por lo cual con los siguientes condicionales nos encargamos de filtrar la información
    //para no tomar los espacios en blanco dependiendo de la fila.

    //Además, como se mencionaba en la explicación del vídeo, los asientos que cuentan con
    //la teconología VibroSound están úbicados en lugares en particular, por lo cual
    //también los tenemos en cuenta dentro de los condicioneles para la fila.

    //Los asientos con tecnología VibroSound, los ■ (el char(254)), son los siguientes:

    //O O O O O O O O O O
    //O O ■ ■ ■ ■ ■ ■ O O
    //O O ■ ■ ■ ■ ■ ■ O O
    //O O ■ ■ ■ ■ ■ ■ O O
    //  O O ■ ■ ■ ■ O O
    //    O O O O O O
    //      O O O O


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

    //Este es el constructor de Show. Está sobrecargado para que cuando se invoque con estos
    //dos argumentos, se cargue desde el archivo de nombre de showID.txt, donde ID es
    //la identificación del show, que conincide con el índice en el que se encuentra almacenado
    //dentro del vector de Shows que se menciona en le explicación dentro del vídeo de Youtube;
    //a manera de ejemplo, el show de id 2 está almacenado en la posición 3 del vector de Shows
    //y su información está almacenada en show2.txt. Estos archivos de texto se encuentran en
    //una carpeta llamada shows dentro de la carpeta data que almacena todas las bases de datos
    //del programa. La variable exists es colocada en true si el show fue cargado exitosamente
    //desde su archivo de texto, o en false si el archivo correspondiente no pudo ser abierto.

    id = _id;
    fstream file("../DesafioEvaluativoV2/data/shows/show" + to_string(id) + ".txt", ios::in);
    if (file.is_open()) {

        exists = true;
        string line;

        //Como se mencionaba en el vídeo, el formato en que se almacena la información dentro
        //de los archivos es el siguiente:

        //NOMBRE DE LA PELÍCULA
        //1 (Si está en 3D) o 0 (en caso contrario)
        //GÉNERO
        //CLASIFICACIÓN (Sin el +, por ejemplo, +16 se almacena como 16)
        //HORA DE LA FUNCIÓN (En formato de 24 horas)
        //HORA DE FINALIZACIÓN (Ver nota)
        //DURACIÓN (En minutos)
        //SALA (Solo se cuentan con 4 salas, la 1, la 2, la 3 y la 4)
        //CANTIDAD DE PUESTOS AÚN SIN RESERVAR
        //MATRIZ DE ASIENTOS EN SU FORMATO DE STRING

        //Nota: La hora de finalización se calcula como la hora en que comienza la película
        //más la cantidad de horas que se tenga que reservar la sala para presentarla,
        //por lo cual si la película dura dos horas y un minuto (ie 121 minutos),
        //como dura más de las dos horas, se debe reservar la sala por 3,
        //luego si la película es presentada a las 14, la hora de
        //finalización será a las 17.

        //Un ejemplo para ilustrar:
        //Si tomamos la película 'Triple Frontera' que se presenta en el cuadro 1 de las
        //instrucciones del examen, el archivo de texto se verá de la siguiente forma:

        //Triple Frontera
        //1
        //Acción
        //16
        //19
        //22
        //125
        //2
        //28
        //00 00 00 00 00 00 00 00 00 00
        //10 00 00 10 00 00 00 10 00 00
        //10 00 01 11 02 12 11 00 10 10
        //10 10 01 02 02 02 02 01 10 10
        //   10 10 11 02 12 11 10 10
        //      10 10 10 00 00 10
        //         10 10 10 10
        //

        //Nota: Como no se especificaba el 3D, lo coloqué como si sí lo fuera en este ejemplo.
        //NOTA IMPORANTE: Simepre debe ir un salto de línea al final del archivo.

        //En este orden de ideas, es fácil entender el resto de código de la método.

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

    //Como mencionamos antes, el constructor está sobrecargar, por lo cual cuando se llama con estos
    //argumentos el Show se construye de forma más sencilla y la matriz seats[][] se inicializa
    //con la sala vacía y todos los asientos siendo ofrecidos en General.

    id =_id; movie_name = _movie_name; is_3D = _is_3D; genre = _genre; clasi = _clasi; hour = _hour;
    finish_hour = _finish_hour; duration = _duration; room = _room; empty_places = _empty_places;

    for (short int row = 0; row < 4; row++) fill_row("10 10 10 10 10 10 10 10 10 10 ", row);
    fill_row("   10 10 10 10 10 10 10 10 ", 4);
    fill_row("      10 10 10 10 10 10 ", 5);
    fill_row("         10 10 10 10 ", 6);
}

void Show::save_show(short int shows_num) const {

    //Guardamos la información de la película en su correspondiente archivo de text,
    //recordemos que si por ejemplo la id de la película es 2, el archivo correspondiente
    //es show2.txt.

    //Notemos que no hace falta validar la existencia del archivo de texto, pues si éste
    //fuese borrado, como es abierto con ios::out se crearía de nuevo.

    //NOTA IMPORTANTE: Si se abre al archivo de texto con bloc de notas por ejemplo,
    //en el lugar donde van los carácteres "raros", como la ñ (eñe) o las letras con tilde
    //en caso de que el nombre de la película o el género los posea, se veran algunos símbolos
    //extraños, pero esto no es de preocuparse pues solo es la interpretación que le da
    //el bloc de notas al binario que se almacenó, pero cuando el archivo es abierto
    //de nuevo con fstream los carácteres son leídos como eran originalmente, por lo cual
    //no hay ningún problema en trabajar con tildes, con ñ (eñe) o carácteres de la tabla
    //ASCII entre el 32 y el 254, inclusive; por carácteres diferentes a estos no puedo
    //dar garantía de un buen funcionamiento.

    //Otro aspecto relevante es que hay un archivo llamado shows_num.txt, que almacena una
    //sola línea con el número de shows que se están ofreciendo actualmente en cartelera,
    //esto es para saber cuantos shows hay de antemano y poder buscar exactamente todos
    //los archivos que contengan la información de algún show, ni más, ni menos.

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

    //Esta parte es para guardar la matriz seats[][].

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

    //NOTA IMPORANTE: Simepre debe ir un salto de línea al final del archivo
    //como mecnionamos antes.

    file.close();
}

bool get_shows(vector<Show> &shows) {

    //Cargamos todos los datos de los shows, desde los archivos de texto, en el vector Show.
    //Retornamos true si los shows pudieron ser cargados exitosamente, o false en caso de que
    //no se haya podído abrir el archivo shows_num.txt o alguno de los archivos con la
    //información de alguna de las películas.

    //Averiguamos la cantidad de shows que debemos cargar consultando el archivo shows_num.txt

    bool exists;
    string line;
    short int shows_num;

    fstream file("../DesafioEvaluativoV2/data/shows/shows_num.txt", ios::in);
    if (file.is_open()) {

        getline(file, line);
        str2int(line, shows_num);

        file.close();

        //Esto se explica más a fondo en el vídeo, pero básicamente es para decirle al vector
        //Shows que reserve espacio para almacenar una cantidad shows_num de películas.

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


//--------------------------------------------------------------------------------------------
//-------------------------MÉTODOS PARA IMPRIMIR EN PANTALLA----------------------------------
//--------------------------------------------------------------------------------------------

void Show::display_row(short int row, const short int &aux, const bool &is_admin) const {

    //Imprime en pantalla la fila row de la matriz seat[][] con un color según
    //lo forma en que estén siendo ofrecidos al público y forma dependiendo
    //de si está vacío u ocupado.

    //Colores: Blanco para General, rojo para VibroSound y amarillo para Gold.
    //Formas: O para asiento vacío, X para asiento ocupado

    //Si is_admin es true imprime los asientos que cuentan con la tecnología
    //VibroSound con la forma ■ (el char(254)).

    //Nota1: Para la elección de la forma la prevalencia es X < ■ < O, es decir,
    //si un asiento está ocupado va con X, si no, si es VibroSound va con ■,
    //o si no es ningúna de las anteriores va con O.

    //Nota2: El color es independiente de la forma.

    for (short int j = aux; j < (10 - aux); j++) {
        cout << char(179) << ' ';
        colored_display(seats[row][j], is_admin);
        cout << ' ';
    }
    cout << char(179);
}

void Show::display_seats(const bool &is_admin) const {

    //Imprime los asientos de la sala de cine donde se presentará
    //la película correspondiente a show.

    //No hace falta tratar de entender el código siguiente, sólo es una
    //forma de mostrar los asientos en una forma que me pareció adecuada.

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

void get_longest_size(const vector<Show> &shows, short int &max_size_name, short int &max_size_genre) {

    //Para poder imprimir la cartelera de películas en pantalla con un formato que
    //me pareciera correcto, decidí implementar una función que determinara la
    //mayor de las longitudes de los nombres de las películas y de los géneros,
    //para poder utilizarlos para imprimer el marco de la cartelera con un tamaño
    //suficientemente grande para contenerlos.

    //Se incilializan en 4 y 5 porque mínimamente deben contener la etiqueta de
    //'Name' y el de 'Genre' respectivamente.

    max_size_name = 4;
    max_size_genre = 5;

    short int len = shows.size();
    for (short int i = 0; i < len; i++) {
        if (max_size_name < short(shows.at(i).get_movie_name().length())) max_size_name = short(shows.at(i).get_movie_name().length());
        if (max_size_genre < short(shows.at(i).get_genre().length())) max_size_genre = short(shows.at(i).get_genre().length());
    }
}

void display_labels(const short int &max_size_name, const short int &max_size_genre) {

    //Imprimimos en pantalla las etiquetas de las columnas de la cartelera,
    //etiquetas como 'Name', 'Genre', '3D, etc.

    display_adapter_separator(201, 203, 187, max_size_name, max_size_genre); cout << endl;
    cout << "   " << char(186); centred_display("ID", 4);
    cout << char(186); centred_display("Name", max_size_name);
    cout << char(186); centred_display("Genre", max_size_genre);
    cout << char(186); centred_display("3D", 6);
    cout << char(186); centred_display("Duration", 10);
    cout << char(186); centred_display("Class.", 8);
    cout << char(186); centred_display("Seats", 7);
    cout << char(186); centred_display("Hour", 10);
    cout << char(186); centred_display("Room", 6);
    cout << char(186);
}

void Show::display_show(const short int &max_size_name, const short int &max_size_genre) const {

    //Imprime en pantalla la información de uno de los shows.

    //Pasamos de horario militar al convencional para ser amigables con el
    //usuario. La fórmula para pasar de horario militar al convencional
    //es: convencional = 1 + (militar - 1)%12, a excepción del 0,
    //por lo cual manejamos este caso particular mediante el uso
    //del operador ternario.

    string str_hour = (hour == 0)?"12":to_string(1 + (hour-1)%12);
    str_hour += (hour < 12)?":00 am":":00 pm";

    cout << "   " << char(186); centred_display(to_string(id + 1), 4);
    cout << char(186); centred_display(movie_name, max_size_name);
    cout << char(186); centred_display(genre, max_size_genre);
    cout << char(186); centred_display(is_3D?"Yes":"No", 6);
    cout << char(186); centred_display(to_string(duration) + " mins", 10);
    cout << char(186); centred_display(to_string(clasi) + "+", 8);
    cout << char(186); centred_display(to_string(empty_places), 7);
    cout << char(186); centred_display(str_hour, 10);
    cout << char(186); centred_display(to_string(room), 6);
    cout << char(186);
}

void display_shows(const vector<Show> &shows) {

    //Imprime la cartelera de películas del vector Shows.

    short int len = shows.size(), max_size_name, max_size_genre;
    get_longest_size(shows, max_size_name, max_size_genre);

    //Sumamos 2 simplementa para que cuando se muestre el nombre de mayor longitud
    //y el género de mayor longitud, display_adapter_separator() coloque dos espacios
    //vacíos en los extremos, es solo cuestión de visualización.

    max_size_name += 2;
    max_size_genre += 2;

    cout << endl;
    display_labels(max_size_name, max_size_genre);
    cout << endl;

    for (short int i = 0; i < len; i++) {
        display_adapter_separator(204, 206, 185, max_size_name, max_size_genre); cout << endl;
        shows.at(i).display_show(max_size_name, max_size_genre); cout << endl;
    }

    display_adapter_separator(200, 202, 188, max_size_name, max_size_genre); cout << endl;
}


//--------------------------------------------------------------------------------------------
//-----------MÉTODOS PARA ADMINISTRAR LOS PROCESOS DE RESERVA Y OFERTA DE ASIENTOS------------
//--------------------------------------------------------------------------------------------

bool Show::get_index(short &index, bool is_row, const bool &is_admin, short int row) {

    //Imprime en pantalla la sala de cine donde se presentará la película. Solicita al
    //usuario el índice de una fila o una columna dependiendo de si is_row es true o
    //false resepectivamente. La variable row es para el caso en que se solicita la
    //columna mostrarle al usuario cual fila había escogido. Si es el administrador
    //quien invoca el método (ie is_admin = true), simplemetne cambiamos los menajes
    //que se muestran al usuario. Retornamos true si el usuario ingresa una fila o
    //columna valida y la almacenamos en la variable index recibida por referencia;
    //o false en caso de que el usuario decida escoger otra película.

    bool ask = true;
    string ans, msg;

    if (is_admin) msg = "  Which seat do you want to offer?";
    else msg = "  Which seat do you want to reserve?";

    system("cls");

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

        //Mostramos la fila escogida si ya estamos escogiendo la columna, para facilidad del usuario.

        if (!is_row) cout << endl << "  CURRENT ROW: " << char(row + 65) << endl;

        cout << "  Enter the " << (is_row?"row":"column") << " of the seat, or an empty space for return: (" << (is_row?'A':'1') << " - " << (is_row?"G":"10") << ") ";
        fflush(stdin);
        getline(cin, ans);

        //Todo esto son sólo condicionales para procesar lo que ingresó el usuario

        if ((ans == "") and yes_no_question("You want to return? (Enter 'Yes' or 'No')")) return false;
        else if (ans == "") system("cls");
        else if (is_row and ((1 < ans.length()) or (!is_in_range(ans[0], 65, 71) and !is_in_range(ans[0], 97, 103)))) {
            msg_and_cls("Sorry, the row must be a letter between A and G.");
        }
        else if (ans == "10") {
            ask = false;
            index = 9;
        }
        else if (!is_row and ((1 < ans.length()) or !is_in_range(ans[0], 49, 57))) {
            msg_and_cls("Sorry, the column must be a number between 1 and 10.");
        }
        else {
            ask = false;

            //Para comodidad del usuario hacemos que el método no sea case sensitive,
            //es decir, si ingresa 'A' o 'a' para las filas reconocemos que se refería
            //a la fila 0 de la matriz seats[][].

            if (is_row and (ans[0] < 72)) index = short(ans[0]) - 65; //Letra a índice - Mayúscula.
            else if (is_row) index = short(ans[0]) - 97; //Letra a índice - Minúscula.
            else index = short(ans[0]) - 49; //Entero positivo a índice - Número de la columna.
        }
    }
    return true;
}

bool is_room_available(const vector<Show> &shows, const short int &room, const short int &hour, short int finish_hour, short int &show_hour, short int &show_finish_hour) {

    //Cuando el admin programa una nueva película, debemos tener en cuenta de que la sala en que
    //se va a proyectar la película esté disponible en el horario ingresado, tanto que no esté
    //siendo proyectada otra pelicula como que no vaya a comenzar a ser proyectada durrante
    //la que se está programando. Está función retorna true si la sala está disponible en
    //el horario ingresado por el admin o false en caso contrario.

    short int len = shows.size();
    for (short int i = 0; i < len; i++) {

        show_hour = shows.at(i).get_hour();
        show_finish_hour = shows.at(i).get_finish_hour();

        if ((shows.at(i).get_room() == room) and ((hour < show_finish_hour) and (show_hour < finish_hour))) return false;
    }
    return true;
}

void Show::modify_offers(short int shows_num, bool &offer) {

    //Esta método administra el resto del proceso para que el admin pueda modificar
    //la forma en que se ofrencen las sillas dentro de las salas, luego de que ya
    //haya escogido cuál función va a modificar.

    string msg;
    bool ask = true;
    short int row, column;

    //Solicitamos la fila y la columna del asiento de la matriz seats[][],
    //al cual el admin le desea cambiar el tipo de modo en que se ofrece.

    while (ask and get_index(row, true, true) and get_index(column, false, true, row)) {

        //Estos sólo son condicionales para procesar lo que el admin ingresó.

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

            //El restamos uno al retorno de la función get_int_input() para poder pasar
            //de un entero positvo a uno de los índices de la matriz.

            seats[row][column].sale_type = get_int_input("In which of them do you want to offer it? (1 - 3)", "\nSorry, we only have these 3 modalities:\n  1. General     2. VibroSound     3. Gold", 1, 3) - 1;
            cout << endl;

            msg_and_cls("The offer has been successfully placed!");
            offer = true;

            //Le preguntamos al admin si desea modificar la forma en que se oferta otro asiento.

            display_seats(true);
            ask = yes_no_question("Do you want to offer other seat? (Enter 'Yes' for offer other seat or 'No' for exit)");
        }
    }
    save_show(shows_num);
}

void Show::reserve_seat(short shows_num, array<unsigned int, 6> &sales, unsigned long long int &total, const unsigned long long int &user_id, bool &reserve, const unsigned int &seed) {

    //Esta método administra el resto del proceso para que el usuario del cine pueda
    //reservar un asiento luego de que ya haya escogido la función de su interés.

    string msg;
    bool ask = true;
    unsigned int price;
    short int row, column, combo = 0;

    //Solicitamos la fila y la columna del asiento de la matriz seats[][],
    //que el usuario desea reservar.

    while (ask and get_index(row, true, false) and get_index(column, false, false, row)) {

        //Procesamos lo que ingresó el usuario.

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

            //En caso de que escoja un asiento válido, es decir, que esté desocupado
            //y sea un asiento de verdad, no como por ejemplo los de las esquinas
            //de la matriz seats[][], procedemos a explicarle en qué consite y
            //cuanto cuesta el tipo de asiento que escogió.

            price = explain_offer_types(seats[row][column], row, column);

            //Aumentamos la tarifa en $3000 en caso de que sea 3D.

            if (is_3D) {
                cout << endl << "  It costs $" << price << ", plus $3000 for the 3D show, for a total of $" << (price + 3000);
                price += 3000;
            }
            else cout << endl << "  It costs $" << price << '.';

            //Confirmamos la reservar del asiento.

            if (yes_no_question("You want to reserve that seat? (Enter 'Yes' or 'No')")) {

                system("cls");
                ask = false;
                cout << endl << "  Reserve seat " << char(row + 65) << (column + 1) << " ----- $" << price << endl;

                //Le solicitamos que ingrese el dinero.

                if (charge_money(price)) {

                    //Y si llegamos a esta parte del código, es porque el usuario decidió
                    //reservar el asiento.

                    //Actualizamos la matriz seats[][] y reducimos los asientos disponibles
                    //de la función.

                    seats[row][column].is_empty = false;
                    empty_places--;

                    //Agregamos el precio de la compra al total de ventas diarias, y registramos
                    //en que modalidad se reservó el asiento para poder discriminar las ventas
                    //por estos tipos, dentro del registro de ventas. Actualizamos el archivo
                    //de texto del registro ventas, sales_record.txt.

                    total += price;
                    if (is_3D) sales.at(seats[row][column].sale_type)++;
                    else sales.at(3 + seats[row][column].sale_type)++;
                    save_sales(sales, total, seed);

                    system("cls");

                    if (seats[row][column].sale_type == 2) {

                        //Y en el caso de que haya escogido un asiento tipo Gold,
                        //le preguntamos cuál combo desea llevar como se explicó
                        //en el vídeo de Youtube.

                        combo = offer_combos();
                        system("cls");
                    }

                    //Actualizamos la información de la cuenta del usuario dentro del archivo
                    //users.txt, con la hora, la sala, el asiento y si llevó un combo almacenamos
                    //cual combo fue, o 0 en caso contrario.

                    update_user(user_id, hour, room, row, column, combo, seed);

                    //Imprimimos un mensaje de agradecimiento y volvemos al inicio para que otro
                    //usuario pueda reservar.

                    cout << endl << "  Thank you for choosing us, we hope you enjoy your movie! :D" << endl << endl << "  ";
                    system("pause");
                    reserve = true;
                }
                else {

                    cout << endl << "  You canceled the reservation" << endl << endl << "  ";
                    system("pause");
                    ask = false;
                }
            }
            else system("cls");
        }
    }
    save_show(shows_num);
}
