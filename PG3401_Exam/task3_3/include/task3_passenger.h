#ifndef TASK3_PASSENGER_H
#define TASK3_PASSENGER_H

#include "task3_struct.h"

/* Legger til en ny passasjer */
Passenger* addPassenger(Passenger* head, int seatNumber, const char* name, int age);

/* Frigjør minnet */
void freePassengerList(Passenger* head);

/* Endrer setet til en passasjer med x navn */
int changeSeat(Flight* head, const char* passengerName, int newSeat);

/* Skrive ut flights der en passasjer med x navn er registert */
void findPassengerFlights(Flight* head, const char* name);

/* Skriver ut passasjerer som er registrert på flere flights */
void findPassengersOnMultipleFlights(Flight* head);

#endif


