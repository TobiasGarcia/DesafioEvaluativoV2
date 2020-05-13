#ifndef SHOW_H
#define SHOW_H

#include <iostream>
#include <vector>

using namespace std;

class Seat {
public:
    bool is_empty = true;
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
    short int finish_hour;
    short int duration;

    short int room;
    Seat seats[7][10];
    short int empty_places;

public:
    void fill_row(string line, short int row);

    //Para cargar el show desde una archivo.
    Show(short int id, bool &exists);

    //Para que el admin cree una show.
    Show(string _movie_name, bool _is_3D, string _genre, short int _clasi, short int _hour,
         short int _finish_hour, short int _duration, short int _room, short int _empty_places);

    void save_show(short int id) const;

    void display_row(short int row, const short int &aux, const bool &is_admin);

    void display_seats(const bool &is_admin);

    short int get_hour() const {return hour;}

    short int get_finish_hour() const {return finish_hour;}

    short int get_room() const {return room;}
};

bool get_shows(vector<Show> &shows);

bool is_room_available(const vector<Show> &shows, const short int &room, const short int &hour, short int finish_hour, short int &show_hour, short int &show_finish_hour);

void add_show(vector<Show> &shows);

void save_shows(const vector<Show> &shows);

void display_shows(vector<Show> shows);

template <typename T>
int ceiling(T num) {
    int int_part = int(num);
    if ((num - int_part) < 0.00000001) return num;
    else return (num + 1);
}




//template <typename T>
//void fun(T a);

template <typename T>
void foo(T a) {
    cout << "General templete: " << a << endl << endl;
}

template<>
void foo(int a) {
    cout << "Specialized templete for int type: " << a << endl << endl;
}

template<>
void foo(char a) {
    cout << "Specialized templete for char type: " << a << endl << endl;
}


#endif // SHOW_H
