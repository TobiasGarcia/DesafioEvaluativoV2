#ifndef SHOW_H
#define SHOW_H

#include <iostream>
#include <vector>
#include <array>

using namespace std;

struct Seat {

    //Cada asiento debería almacenar principalmente 3 cosas, una bandera para saber si
    //está vacío u ocupado, otra bandera para saber si cuenta con la tecnología VibroSound,
    //y un entero que almacenará la forma en que es ofresido al público. Recordemos que
    //como se mencionaba en la explicación del vídeo de Youtube, hay 3 tipos, General,
    //VibroSound y Gold, los cuales corresponden a 0, 1 y 2 respectivamente; pero además
    //como se explicó también, en la matriz de asientos hay espacios que no son asientos
    //en realidad, por lo cual, para identificarlos almacenamos -1 en sale_type.

    //Obtamos por utilizar una estrucuta en lugar de una clase pues no necesitamos métodos,
    //sólo el poder construir un "array" pero de elementos de diferente tipo.

    //Nota: Preferí no almacenar detro de la estructura asiento la ID de la persona que lo reservó,
    //esto es porque haría mucho más complicado el manejo de la base de datos de las salas, por lo
    //cual la información de quien reservó el asiento es almacenada en cada una de las cuentas
    //de usuarios dentro del archivo users.txt, el cual está codificado por seguridad.

    bool is_empty = true;
    bool is_vibro = false;
    short int sale_type = -1;
};

class Show {

    //Esta es la clase para abstraer las películas que presentará el cine.
    //La mayoría de atributos se explican solo con su nombre, a excepción
    //de id, que corresponde al índice que ocupa la película dentro el que será
    //el vector que almacenará la oferta de películas, Shows; seats[][]
    //que será una matriz que almacenará los asientos de cada show,
    //y clasi que almacenará la clasificación de la película +14, +16, etc.

    //Nota: Una explicación detallada del propósito del atributo finish_hour
    //se expone en la documentación del método display_show() dentro de show.cpp.

private:

    short int id;
    string movie_name;

    bool is_3D;
    string genre;
    short int clasi;

    short int hour;
    short int finish_hour;
    short int duration;

    short int room;
    Seat seats[7][10];
    short int empty_places;

public:

    //--------------------------------------------------------------MÉTODOS GETTERS DE SHOW

    string get_movie_name() const {return movie_name;}

    string get_genre() const {return genre;}

    short int get_hour() const {return hour;}

    short int get_finish_hour() const {return finish_hour;}

    short int get_room() const {return room;}

    short int get_empty_places() const {return empty_places;}

    //----------------------------------------------------------------------MÉTODOS DE SHOW

    void fill_row(string line, short row);

    //Para cargar el show desde una archivo.
    Show(short int _id, bool &exists);

    //Para que el admin cree un show.
    Show(short int _id, string _movie_name, bool _is_3D, string _genre, short int _clasi, short int _hour,
         short int _finish_hour, short int _duration, short int _room, short int _empty_places);

    void save_show(short int shows_num) const;

    void display_row(short int row, const short int &aux, const bool &is_admin) const;

    void display_seats(const bool &is_admin) const;

    void display_show(const short int &max_size_name, const short int &max_size_genre) const;

    bool get_index(short &index, bool is_row, const bool &is_admin, short int row = 0);

    void modify_offers(short int shows_num, bool &offer);

    void reserve_seat(short shows_num, array<unsigned int, 6> &sales, unsigned long long int &total, const unsigned long long int &user_id, bool &reserve, const unsigned int &seed);
};

bool get_shows(vector<Show> &shows);

void get_longest_size(const vector<Show> &shows, short int &max_size_name, short int &max_size_genre);

void display_shows(const vector<Show> &shows);

bool is_room_available(const vector<Show> &shows, const short int &room, const short int &hour, short int finish_hour, short int &show_hour, short int &show_finish_hour);

void display_labels(const short int &max_size_name, const short int &max_size_genre);


//-----------------------------------------------------------------------------FUNCIONES PLANTILLA

template <typename T>
int ceiling(T num) {

    //Como su nombre se indica, es la función techo, que devuelve el mínimo
    //de los números enteros mayores o iguales que el ingresado.

    int int_part = int(num);
    if ((num - int_part) < 0.00000001) return num;
    else return (num + 1);
}

#endif // SHOW_H
