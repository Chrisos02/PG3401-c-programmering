#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "task3_struct.h"
#include "task3_flight.h"
#include "task3_passenger.h"


//============================================================
// Legger til ny flight bakerst i den dobbeltlenkede listen
//============================================================
Flight* addFlight(Flight* head, const char* flightId, const char* destination, int seats, int time) {
    Flight* newFlight = (Flight*)malloc(sizeof(Flight));
    if (!newFlight) {
        printf("Error allocating memory for flight.\n");
        return head;
    }

    /* Kopierer flight info inn i strukturen */
    strncpy(newFlight->flightId, flightId, FLIGHTID_LENGTH - 1);
    newFlight->flightId[FLIGHTID_LENGTH - 1] = '\0';

    strncpy(newFlight->destination, destination, DEST_LENGTH - 1);
    newFlight->destination[DEST_LENGTH - 1] = '\0';

    /* Setter resterende verdier */
    newFlight->seats = seats;
    newFlight->time = time;
    newFlight->passengerList = NULL;
    newFlight->prev = NULL;
    newFlight->next = NULL;

    /* Dersom listen er tom, returner ny flight som head */
    if (!head) {
        return newFlight;
    }

    Flight* current = head;
    while (current->next) {
        current = current->next;
    }
    current->next = newFlight;
    newFlight->prev = current;

    return head;
}

//=================================================
// Skriver ut detaljer for flight basert på indeks
//=================================================
void printFlightDetailsAtIndex(Flight* head, int index) {
    int count = 1;
    while (head != NULL && count < index) {
        head = head->next;
        count++;
    }

    if (head == NULL) {
        printf("No flight at index %d\n", index);
        return;
    }

    /* Flight info */
    printf("Flight #%d: %s -> %s | Seats: %d | Time: %d\n",
           index, head->flightId, head->destination, head->seats, head->time);

    /* Skriver ut passasjerlsite */
    Passenger* passenger = head->passengerList;
    while (passenger != NULL) {
        printf("  Seat %d: %s (%d years)\n", passenger->seatNumber, passenger->name, passenger->age);
        passenger = passenger->next;
    }
}

//=======================================================================
// Søker etter flight med gitt destinasjon og returnerer 1 basert indeks
//=======================================================================
int findFlightByDestination(Flight* head, const char* destination) {
    int index = 1;
    while (head != NULL) {
        if (strcmp(head->destination, destination) == 0) {
            return index;
        }
        head = head->next;
        index++;
    }
    /* Klarte ikke finne destinasjon */
    return -1;
}


//==================================================================
// Sletter flight ved gitt indeks og frigjør tilknyttede passasjerer
//==================================================================
Flight* deleteFlight(Flight* head, int index) {
    if (head == NULL || index < 1) {
        return head;
    }

    Flight* current = head;
    int count = 1;

    /* Går til valgt indeks */
    while (current != NULL && count < index) {
        current = current->next;
        count++;
    }

    if (current == NULL) {
        printf("No flight at index %d to delete.\n", index);
        return head;
    }

    /* Frigjør passasjerlisten til flighten */
    freePassengerList(current->passengerList);

    /* Kobler ut flight fra listen */
    if (current->prev) {
        current->prev->next = current->next;
    } else {
        head = current->next;
    }

    if (current->next) {
        current->next->prev = current->prev;
    }

    free(current);
    return head;
}

//============================================
// Skriver ut alle flights og passasjerlister
//============================================
void printFlights(Flight* head) {
    int count = 1;
    while (head) {

        /* Flight info */
        printf("Flight #%d: %s -> %s | Seats: %d | Time: %d\n",
               count, head->flightId, head->destination, head->seats, head->time);

        /* Skriver ut tilhørende passasjerer */
        Passenger* p = head->passengerList;
        while (p) {
        printf("  Seat %d: %s (%d years)\n", p->seatNumber, p->name, p->age);
        p = p->next;
        }

        head = head->next;
        count++;
    }
}

/* End of file */







