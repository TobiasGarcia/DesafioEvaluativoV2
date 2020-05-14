#ifndef SHOW_H
#define SHOW_H

#include <iostream>
#include <vector>
#include <array>

using namespace std;

class Seat {
public:
    bool is_empty = true;
    bool is_vibro = false;
    short int sale_type = -1;
};

class Show {
private:
    short int id;
    string movie_name;

    bool is_3D;
    string genre;
    short int clasi; //La clasificación de la película

    short int hour;
    short int finish_hour;
    short int duration;

    short int room;
    Seat seats[7][10];
    short int empty_places;

public:
    void fill_row(string line, short int row);

    //Para cargar el show desde una archivo.
    Show(short int _id, bool &exists);

    //Para que el admin cree una show.
    Show(short int _id, string _movie_name, bool _is_3D, string _genre, short int _clasi, short int _hour,
         short int _finish_hour, short int _duration, short int _room, short int _empty_places);

    void save_show(short int shows_num) const;

    void display_row(short int row, const short int &aux, const bool &is_admin) const;

    void display_seats(const bool &is_admin) const;

    string get_movie_name() const {return movie_name;}

    string get_genre() const {return genre;}

    short int get_hour() const {return hour;}

    short int get_finish_hour() const {return finish_hour;}

    short int get_room() const {return room;}

    short int get_empty_places() const {return empty_places;}

    void display_show(const short int &size1, const short int &size2) const;

    bool get_index(short &index, bool is_row, const bool &is_admin, short int row = 0);

    void offer_seats(short int shows_num);

    void reserve_seat(short shows_num, array<unsigned int, 6> &sales, unsigned long long int &total, const unsigned long long int &user_id, const unsigned int &seed);
};

bool get_shows(vector<Show> &shows);

bool is_room_available(const vector<Show> &shows, const short int &room, const short int &hour, short int finish_hour, short int &show_hour, short int &show_finish_hour);

void add_show(vector<Show> &shows);

void save_shows(const vector<Show> &shows);

void display_shows(const vector<Show> &shows);

template <typename T>
int ceiling(T num) {
    int int_part = int(num);
    if ((num - int_part) < 0.00000001) return num;
    else return (num + 1);
}

//template <typename T>
//void fun(T a);

#endif // SHOW_H
