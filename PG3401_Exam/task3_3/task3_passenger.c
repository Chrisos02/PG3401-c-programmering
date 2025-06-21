#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "task3_struct.h"
#include "task3_passenger.h"

//===================================================
// Legger til en passasjer i en singel-linked liste
// Sorterer listen etter setenummer
// Sjekker for duplikat setenummer
//===================================================
Passenger* addPassenger(Passenger* head, int seatNumber, const char* name, int age) {
    Passenger* newNode = malloc(sizeof(Passenger));
    if (!newNode) {
        printf("Memory allocation failed.\n");
        return head;
    }

    /* Sett passasjerens data */
    newNode->seatNumber = seatNumber;
    strncpy(newNode->name, name, NAME_LENGTH - 1);
    newNode->name[NAME_LENGTH - 1] = '\0';
    newNode->age = age;
    newNode->next = NULL;

    /* Sett først i lista hvis den er tom */
    if (!head || seatNumber < head->seatNumber) {
        newNode->next = head;
        return newNode;
    }

    /* Finn riktig plass */
    Passenger* current = head;
    while (current->next && current->next->seatNumber < seatNumber) {
        current = current->next;
    }

    /* Sjekker for duplikat sete */
    if (current->seatNumber == seatNumber || (current->next && current->next->seatNumber == seatNumber)) {
        printf("Seat number %d already taken.\n", seatNumber);
        free(newNode);
        return head;
    }

    newNode->next = current->next;
    current->next = newNode;

    return head;
}

//=========================
// Frigjør passasjerlisten
//=========================
void freePassengerList(Passenger* head) {
    Passenger* temp;
    while (head) {
       temp = head;
       head = head->next;
       free(temp);
    }
}


//===========================================================================
// Søker gjennom alle flights etter gitt passasjernavn
// Endrer setenummer hvis passasjer blir funnet
// Returnerer 1 hvis endret, 0 hvis ikke
//===========================================================================
int changeSeat(Flight* head, const char* passengerName, int newSeat) {
    while (head != NULL) {
         Passenger* p = head->passengerList;
         while (p != NULL) {
         if (strcmp(p->name, passengerName) == 0) {
              p->seatNumber = newSeat;
              return 1;
            }
            p = p->next;
        }
        head = head->next;
    }
    return 0;
}


//========================================================
// Skriver hvilke flights en passasjer er registrert på
//========================================================
void findPassengerFlights(Flight* head, const char* name) {
    int found = 0;
    while (head != NULL) {
    Passenger* p = head->passengerList;
    while (p != NULL) {
      if (strcmp(p->name, name) == 0) {
            printf("Passenger %s is on flight %s to %s\n", name, head->flightId, head->destination);
             found = 1;
             break;
            }
            p = p->next;
        }
        head = head->next;
    }
     if (!found) {
        printf("Passenger %s not found on any flight.\n", name);
    }
}


//===========================================================================
// Skriver ut navn på passasjer som er registrert på flere flights
//===========================================================================
void findPassengersOnMultipleFlights(Flight* head) {
    typedef struct {
        char name[NAME_LENGTH];
        int count;
    } NameEntry;

    NameEntry seen[100];
    int seenCount = 0;

    Flight* f = head;
    while (f) {
        Passenger* p = f->passengerList;
        while (p) {
            int found = 0;

         /* Sjekker om navnet allerede er registrert */
         for (int i = 0; i < seenCount; i++) {
         if (strcmp(seen[i].name, p->name) == 0) {
                seen[i].count++;
                  found = 1;
                   break;
               }
            }

            /* Hvis ikke registrert, legg til */
            if (!found) {
                strncpy(seen[seenCount].name, p->name, NAME_LENGTH);
                seen[seenCount].count = 1;
                seenCount++;
            }
            p = p->next;
        }
        f = f->next;
    }

    /* Skriv ut passasjerer som er på flere flights */
    printf("\nPassengers booked on multiple flights:\n");
    int any = 0;
    for (int i = 0; i < seenCount; i++) {
    if (seen[i].count > 1) {
          printf("  %s (%d flights)\n", seen[i].name, seen[i].count);
          any = 1;
      }
    }

    if (!any) {
        printf(" No passengers booked on more than one flight.\n");
    }
}

/* End of file */









