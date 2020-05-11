#ifndef MOVIE_ROOM_H
#define MOVIE_ROOM_H

#include <iostream>

using namespace std;

class Seat {
public:
    bool is_empty = true;
    bool is_vibro;
    int type;
};

class Room {
private:
    int id;
    Seat seats[7][10];
    int empty_places = 58;
public:
    Room(int _id);
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
