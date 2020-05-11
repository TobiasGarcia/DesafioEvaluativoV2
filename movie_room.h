#ifndef MOVIE_ROOM_H
#define MOVIE_ROOM_H

#include <iostream>

using namespace std;

class Seat {
public:
    bool is_empty = false;

    bool is_vibro = false;

    short int sale_type = -1;
};

class Room {
private:
    int id;

    Seat seats[7][10];

    short int empty_places;

public:
    Room(int _id);

    void save_room();

    int get_id() {return id;}

    int get_empty_places() {return empty_places;}

    void display_row(short int row, const short int &aux, const bool &is_admin);

    void display_seats(const bool &is_admin);
};

class Movie {
private:
    string name;

};

#endif // MOVIE_ROOM_H
