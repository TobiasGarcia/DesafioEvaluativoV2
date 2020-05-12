#ifndef SHOW_H
#define SHOW_H

#include <iostream>
#include <vector>

using namespace std;

class Seat {
public:
    bool is_empty = false;
    bool is_vibro = false;
    short int sale_type = -1;
};

class Show {
private:
    string movie_name;

    bool is_3D;
    string genre;
    short int clasi; //La clasificación de la película

    short int hour;
    short int duration;

    short int room;
    Seat seats[7][10];
    short int empty_places;

public:
    void fill_row(string line, short int row);

    //Para cargar el show desde una archivo.
    Show(short int id, bool &exists);

    //Para que el admin cree una show.
    Show(string _movie_name, bool _is_3D, string _genre, short int _clasi,
         short int _hour, short int _duration, short int _room, short int _empty_places);

    void save_show(short int id);

    void display_row(short int row, const short int &aux, const bool &is_admin);

    void display_seats(const bool &is_admin);
};

bool get_shows(vector<Show> &shows);

#endif // SHOW_H
