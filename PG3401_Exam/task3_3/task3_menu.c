#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "task3_struct.h"
#include "task3_flight.h"
#include "task3_passenger.h"
#include "task3_menu.h"

/* runMenu håndterer menyen for flight managment*/
void runMenu(Flight** flightList) {
    int choice;
    do {
	/* Skriver ut flight menu med tilgjengelige alternativer */
	printf("\nFlight Management Menu:   \n");
        printf("1. List all flights\n");
        printf("2. Add new flight\n");
        printf("3. Add passenger to flight\n");
        printf("4. Change passenger seat\n");
        printf("5. Delete a flight\n");
        printf("6. Find flight by destination\n");
        printf("7. Print flight details by index\n");
	printf("8. Search passenger by name\n");
	printf("9. Find passanger on multiple flights\n");
        printf("0. Exit\n");
        printf("Enter choice: ");
        scanf("%d", &choice);
        getchar();

        /* Håndterer brukerens valg ved hjelp av switch case */
        switch (choice) {
            case 1:
	        /* Skriver ut flights */
                printFlights(*flightList);
                break;

            case 2: {
	        /* Legger til et nytt flight */
                char id[FLIGHTID_LENGTH], dest[DEST_LENGTH];
                int seats, time;
                printf("Flight ID: ");
                fgets(id, sizeof(id), stdin); strtok(id, "\n");
                printf("Destination: ");
                fgets(dest, sizeof(dest), stdin); strtok(dest, "\n");
                printf("Seats: "); 
		scanf("%d", &seats);
                printf("Time: "); 
		scanf("%d", &time); 
		getchar();

                 /* Legger til den nye flighten bakerst i listen*/
                *flightList = addFlight(*flightList, id, dest, seats, time);
                break;
            }

            case 3: {
	        /* Legg til passasjer i valgt flight */
                int index, seat, age;
                char name[NAME_LENGTH];
                printf("Flight index to add passenger to: ");
                scanf("%d", &index); getchar();
                printf("Name: "); 

                /* Leser inn navnet til passasjeren */
	        fgets(name, sizeof(name), stdin); strtok(name, "\n");
                printf("Seat: "); 
		scanf("%d", &seat);
                printf("Age: "); 
		scanf("%d", &age); 
		getchar();

                Flight* f = *flightList;
                for (int i = 1; f && i < index; i++) 
		    f = f->next;

                if (f) 
		f->passengerList = addPassenger(f->passengerList, seat, name, age);
                else printf("Flight not found.\n");
                break;
            }

            case 4: {
	        /* Endre setenummer for en passasjer */
                char name[NAME_LENGTH];
                int seat;
                printf("Passenger name: ");
                fgets(name, sizeof(name), stdin); strtok(name, "\n");
                printf("New seat: "); 
	        scanf("%d", &seat); 
		getchar();
                
                /* Oppdaterer setenummeret dersom passasjeren finnes */
	        if (changeSeat(*flightList, name, seat))
                    printf("Seat changed.\n");
                else
                    printf("Passenger not found.\n");
                break;
            }

            case 5: {
                /* Sletter en flight på listen basert på indeks */
                int index;
                printf("Flight index to delete: ");
                scanf("%d", &index); getchar();
                *flightList = deleteFlight(*flightList, index);
	        printf("Flight deleted.\n");
                break;
            }

            case 6: {
	        /* Søkwe etter flight basert på destination */
                char dest[DEST_LENGTH];
                printf("Destination to search: ");
                fgets(dest, sizeof(dest), stdin); strtok(dest, "\n");
                int pos = findFlightByDestination(*flightList, dest);
                
	        if (pos != -1)
                    printf("Flight to %s is number %d in the list\n", dest, pos);
                else
                    printf("Flight not found.\n");
                break;
            }

            case 7: {
	        /* Skriver ut detaljer om flight etter gitt indeks */
                int index;
                printf("Flight index to show: ");
                scanf("%d", &index); getchar();
                printFlightDetailsAtIndex(*flightList, index);
                break;
            }

	    case 8: {
                /* Søker etter passasjernavn og viser hvilke flights de er på */
    		char name[NAME_LENGTH];
    		printf("Enter passenger name: ");
    		fgets(name, sizeof(name), stdin); strtok(name, "\n");
    		findPassengerFlights(*flightList, name);
    		break;
            }

	    case 9: {
              /* Finner passasjerer registrert på flere flights */
              findPassengersOnMultipleFlights(*flightList);
              break;
	    }

            case 0:
                /* Avslutt menyen */
                printf("Exiting.\n");
                break;

            default:
                printf("Invalid choice, please try again.\n");
        }

    } while (choice != 0);
}

/* End of file */



