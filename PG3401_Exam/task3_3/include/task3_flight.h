#ifndef TASK3_FLIGHT_H
#define TASK3_FLIGHT_H

#include "task3_struct.h"

/* Legger til ny flight i double-linked lsiten */
Flight* addFlight(Flight* head, const char* flightId, const char* destination, int seats, int time);

/* Printer ut alle flights + passasjerinfo */
void printFlights(Flight* head);

/* Skriver ut detaljer for flight baser på indeks */
void printFlightDetailsAtIndex(Flight* head, int index);

/* Søker etter flights basert på destinasjon */
int findFlightByDestination(Flight* head, const char* destination);

/* Sletter flights ved gitt indeks og gir ny liste */
Flight* deleteFlight(Flight* head, int index); 

#endif

