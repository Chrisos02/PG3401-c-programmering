#ifndef TASK3_STRUCT_H
#define TASK3_STRUCT_H

#define FLIGHTID_LENGTH 10
#define DEST_LENGTH 50
#define NAME_LENGTH 50


/* Struktur for passasjer */
typedef struct Passenger {
    int seatNumber;
    char name[NAME_LENGTH];
    int age;
    struct Passenger* next;

} Passenger;


/* Struktur for flight */
typedef struct Flight {
    char flightId[FLIGHTID_LENGTH];
    char destination[DEST_LENGTH];
    int seats;
    int time;
    struct Flight* prev;
    struct Flight* next;
    Passenger* passengerList;

} Flight;


#endif

