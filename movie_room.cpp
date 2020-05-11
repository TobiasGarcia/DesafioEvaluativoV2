#include "movie_room.h"
#include <fstream>
#include <windows.h>
#include "dialog.h"

using namespace std;

//Esta son las implementaciones de los métodos que permitiran interactuar con los objetos
//room y movie. Pero antes de las implementaciones hay una explicación de los tipos de sillas que
//ofrecerá el cine y la forma en que se administrarán los asientos en cada sala.

//Cada sala de cine tendra la siguiente estructura:

//                             Fgura 1
//     0 1 2 3 4 5 6 7 8 9
//
// 0   O O O O O O O O O O
// 1   O O ■ ■ ■ ■ ■ ■ O O
// 2   O O ■ ■ ■ ■ ■ ■ O O
// 3   O O ■ ■ ■ ■ ■ ■ O O
// 4     O O ■ ■ ■ ■ O O
// 5       O O O O O O
// 6         O O O O

//Donde las Os representan asientos normales, y los ■s representan asientos que cuentan
//con la tecnología de VibroSound. Esta estructura será almacenada en una matriz de
//objetos tipo Seat, llamada seats. Cada objeto Seat tendrá dos banderas, is_empty
//e is_vibro, las cuales dirán si el asiento está vacío y si cuenta con la teconología
//VibroSound, respectivamente; otra variable llamada sale_type, que almacenará un entero
//correspondiente a la forma en que se oferta esa silla a los usuarios del cine.

//Los 3 tipos de asientos que ofrece el cine son:
//General, VibroSound o Gold. El 2D o 3D dependen de la función, no del asiento, por lo
//cual no lo consideraremos como un tipo de éstos, pero si lo tendremos en cuenta a la
//hora de que el usuario escoja la función.

//sale_type puede tomar los valores de -1 0 1 2, donde -1 es para identificar que esa
//parte de la matriz seats no es en realidad un asiento, como por ejemplo seats[1][6];
//0 será para ofrecer ese asiento en General, 1 para ofrecerlo en VibroSound y 2 para
//Gold.

//El asiento General es un asiento sencillo, sin nada especial en particular; el asiento
//VibroSound, como su nombre lo indica, cuenta con el servicio de VibroSound, que es
//comunmente ofrecido por los cines modernos; y el asiento Gold cuenta con el
//servicio VibroSound, pero además, al comprar el boleto el usuario podrá elegir
//un combo del servicio de comidas para que se lo lleven a la sala, la ventaja
//es que cuando se compra el boleto se paga de una vez el combo y se lo llevarán a
//la silla antes de que comience la función, por lo cual el usuario se ahorra el
//tener que hacer filas y podría ser que el combo saliera más barato si se compra
//de esta manera, aunque esto último ya son detalles que se acordarían con el cliente
//en un esenario de la vida real.

//Nota: La parte de pedir el combo en el asiento Gold es solo para explicar qué
//lo diferencia de los otros asientos, en realidad para poder lograr la parte
//de elegir el combo al comprar la boleta se tendría que integrar éste sistema
//junto con el sistema de administración de venta de alimentos, pero eso
//podría ser una actualización futura del proyecto, por ahora sólo
//ofertaremos cuatro combos cualesquiera.

//Notemos que de esta forma todas las sillas con O solo podrán ser ofertadas en General,
//mientras que las sillas con ■ podrán ser ofertadas en General, VibroSound o Gold,
//y el admin es el que decide como ofertar estos 3 tipo de sillas según la
//disponibilidad de cuales de éstas no están vendidas aún y cuales poseen la
//tecnología del VibroSound.

Room::Room (int _id) {

    id = _id;

    string line;
    short int i = 0;

    //El cine contará con cuatro salas, con IDs 1, 2, 3, y 4. La el txt correpondiente a la
    //información de una sala estará almacenado en la carpeta del proyecto, DesafioEvaluativoV2
    //dentro de una carpeta llamda rooms dentro de la carpeta llamada data. Cada archivo
    //tendrá el nombre de 'roomN.txt', donde N es la ID de la sala.

    fstream file("../DesafioEvaluativoV2/data/rooms/room" + to_string(id) + ".txt", ios::in);
    while (i < 7) {

        getline(file, line);

        //Como podemos ver en la figura 1, hay espacios de la matriz que no
        //son asientos, por lo cual inicializamos la matriz como si ninguno fuera
        //asiento y vamos agregando los que sí. Como la información de los espacios
        //que no son asientos no es de interés, no hay necesidad de guardarla en
        //el txt, por lo cual cada renglón sólo contiene la información de los
        //espacios que sí son asientos dentro de la matriz.

        //Dentro del txt la información de cada asiento será representada por dos
        //enteros, el primero puede ser 0 o 1, y representa si el asiento está
        //vendido o no; mientras que el segundo representa la forma en que se
        //está ofertando el asiento, 0 para General, 1 para VibroSound y 2 para
        //Gold. Los asientos están separados por un espacio entre ellos. En el
        //último renglón del txt habrá un número que representa la cantidad
        //de asientos disponilbes, sin diferenciar entre General, VibroSound
        //o Gold. Un ejemplo del txt de una sala podría ser el siguiente:

        //10 10 10 10 00 00 10 10 10 10
        //10 10 10 10 10 10 10 10 10 10
        //00 00 10 11 01 01 11 10 10 10
        //00 00 10 01 01 11 11 10 00 00
        //10 10 12 12 02 02 10 10
        //10 10 10 10 10 10
        //10 10 10 10
        //42

        //Más adelante en el método display_seats() hablaremos un poco mejor de como
        //se puede interpretar esta información.

        //El siguiente código sólo es para sacar la información almacenada en el txt.
        //Recorre cada renglón del txt y toma la información para llenar la matriz
        //seats.

        if (i == 0) {
            for (short int j = 0; j < 10; j++) {
                seats[i][j].is_empty = (line[3*j] == '1');
                seats[i][j].sale_type = int(line[1 + 3*j]) - 48;
            }
        }
        else if ((0 < i) and (i < 4)) {
            for (short int j = 0; j < 10; j++) {
                seats[i][j].is_empty = (line[3*j] == '1');
                seats[i][j].sale_type = int(line[1 + 3*j]) - 48;
                if ((1 < j) and (j < 8)) seats[i][j].is_vibro = true;
            }
        }
        else if (i == 4){
            for (short int j = 1; j < 9; j++) {
                seats[i][j].is_empty = (line[3*(j - 1)] == '1');
                seats[i][j].sale_type = int(line[1 + 3*(j - 1)]) - 48;
                if ((2 < j) and (j < 7)) seats[i][j].is_vibro = true;
            }
        }
        else {
            for (short int j = (i - 3); j < (13 - i); j++) {
                seats[i][j].is_empty = (line[3*(j + 3 - i)] == '1');
                seats[i][j].sale_type = int(line[1 + 3*(j + 3 - i)]) - 48;
            }
        }
        i++;
    }

    //Almacenamos la cantidad de asientos disponibles en la variable empty_places.

    getline(file, line);
    str2int(line, empty_places);
    file.close();
}

void Room::save_room() {

    //Este método almacena la información de la matriz seats en el archivo txt correspondiente
    //a la ID de la sala en el formato que se explico anterioromente.

    string seat;
    short int j_min = 0, j_max = 10;

    fstream file("../DesafioEvaluativoV2/data/rooms/room" + to_string(id) + ".txt", ios::out);
    for (short int i = 0; i < 7; i++) {

        if (3 < i) {
            j_min = i - 3;
            j_max = 13 - i;
        }

        for (short int j = j_min; j < j_max; j++) {
            seat = "";
            seat.push_back(seats[i][j].is_empty?'1':'0');
            seat.push_back(char(seats[i][j].sale_type + 48));
            seat.push_back(' ');
            file << seat;
        }
        file << '\n';
    }

    //Añadimos la cantidad de asientos disponibles.

    file << empty_places;
    file.close();
}

//Ahora vamos con la parte de imprimir la información del matriz seats
//en pantalla.

void display_wall(short int left, short int walls, short int right) {

    //Esto es solo para imprimir una pared del cine, left determina cuantos espacios vacíos dejamos
    //a la izquierda, walls la cantidad de espacios, o más bien el espasor de la pared, y right
    //la cantidad de espacios vacíos luego de la pared.

    for (short int j = 0; j < left; j++) cout << ' ';
    for (short int j = 0; j < walls; j++) cout << char(177);
    for (short int j = 0; j < right; j++) cout << ' ';
}

void colored_display(const Seat &seat, const bool &is_admin) {

    //Los asientos ocupados serán impresos con una X, mientras que los disponibles
    //con un O. Si el admin es el que desea ver la sala de cine, el tiene que poder
    //identificar cuales sillas cuentan con tecnología VibroSound, por lo cual
    //si is_admin es true, las sillas que cuentann con VibroSound son impresas
    //con un ■ en caso de no estar vendidas aún. Por otra parte,
    //independientemente de si está ocuapada o no, cuenta con
    //VibroSound o no, el color determina en que forma se está
    //ofertando la silla a los usuarios, blanco es en General,
    //rojo es en VibroSound, y amarillo para Gold.

    //Recordemos que una silla con tecnología VibroSound puede ser ofrecida como
    //General, VibroSound o Gold, mientras que una silla sin tecnología VibroSound
    //sólo puede ser ofertada en General.

    char chr;
    if (!seat.is_empty) chr = 'X';
    else if (is_admin and seat.is_vibro) chr = char(254); //Éste carácter es el ■.
    else chr = 'O';

    //Recordemos que los enteros diferente de 0 siempre arroja true al ser juzgados
    //dentro de un condicional, luego el if() se cumplira si el asiento se oferta
    //en 1 (VibroSound) o 2 (Gold), por lo cual le debemos cambiar el color.

    if (seat.sale_type) {

        //Honestamente éste código lo tuve que copiar y pegar de internet,
        //más especificamente de éste foro:

        //https://stackoverflow.com/questions/4053837/colorizing-text-in-the-console-with-c

        //Se utiliza para poder manejar colores en consola.

        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

        SetConsoleTextAttribute(hConsole, (seat.sale_type == 2)?6:12);
        cout << chr;
        SetConsoleTextAttribute(hConsole, 7);
    }
    else cout << chr;
}

void Room::display_row(short int row, const short int &aux, const bool &is_admin) {

    //Éste método imprime en pantalla la fila row de la matriz seats, aux es para
    //identifiacar si hay que mostrar espacios antes de los asientos como sucede
    //en las filas 4, 5 y 6. La variable is_admin es para saber si es el admin
    //quien solicita ver la sala de cine, es para poder imprimir los asientos
    //con tecnología VibroSound con los ■s.

    for (short int j = aux; j < (10 - aux); j++) {
        cout << char(179) << ' ';
        colored_display(seats[row][j], is_admin);
        cout << ' ';
    }
    cout << char(179);
}

void display_separator(short int size, short int chr1, short int chr2, short int chr3) {

    //Ésta función sólo es con el propósito de ahorrar el tener que reescribir el código para imprimir
    //cada separación de las filas de la sala de cine, debido a que estos códigos solo difieren en
    //los valores de algunos carácteres.

    cout << char(chr1);
    for (int i = 0; i < (size - 1); i++) cout << char(196) << char(196) << char(196) << char(chr2);
    cout << char(196) << char(196) << char(196) << char(chr3);
}

void Room::display_seats(const bool &is_admin) {

    //Éste método imprime en pantalla la sala de cine.

    short int aux = 0;

    cout << endl << endl;
    display_wall(4, 47, 0); cout << endl;

    display_wall(4, 2, 1); display_separator(10, 218, 194, 191); display_wall(1, 2, 0); cout << endl;
    display_wall(4, 2, 1); display_row(0, 0, is_admin); display_wall(1, 2, 0); cout << endl;

    for (short int i = 1; i < 7; i++) {

        if (3 < i) {
            display_wall(4, 2, 1 + 4*aux); display_separator(10 - 2*aux, 192, 197, 217); display_wall(1 + 4*aux, 2, 0); cout << endl;
            aux = i - 3;
        }
        else {
            display_wall(4, 2, 1); display_separator(10, 195, 197, 180); display_wall(1, 2, 0); cout << endl;
        }
        display_wall(4, 2, 1 + 4*aux); display_row(i, aux, is_admin); display_wall(1 + 4*aux, 2, 0); cout << endl;
    }
    display_wall(4, 2, 13); display_separator(4, 192, 193, 217); display_wall(13, 2, 0); cout << endl;

    display_wall(4, 2, 22); display_wall(21, 2, 0); cout << endl;
    display_wall(4, 2, 22); display_wall(21, 2, 0); cout << endl;
    display_wall(4, 2, 22); display_wall(21, 2, 0); cout << endl;

    display_wall(4, 2, 5); cout << char(177); for (short int j = 0; j < 31; j++) cout << char(178); cout << char(177); display_wall(5, 2, 0); cout << endl;
    display_wall(4, 47, 0); cout << endl;
    cout << endl;
}








