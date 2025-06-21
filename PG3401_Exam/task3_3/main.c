//===========================================
// Task 3 - PG3401 Exam                                       
// List Handling       
// Author: Candidate Number 3                                   
// Date: May 2025                                  
//===========================================

#include <stdio.h>
#include <stdlib.h>
#include "task3_struct.h"
#include "task3_flight.h"
#include "task3_passenger.h"
#include "task3_menu.h"

int main() {
	Flight* flightList = NULL;
    
	flightList = addFlight(flightList, "SK103", "OSLO", 120, 1030);
	flightList = addFlight(flightList, "DY456", "BERGEN", 100, 1200);
        flightList = addFlight(flightList, "SK489", "STAVANGER", 80, 1630);
        flightList = addFlight(flightList, "DA199", "LONDON", 140, 1830);

	/* Starter menyen */
	runMenu(&flightList);

	/* Frigjør allokert minne for flights og passasjerliste */
	Flight* temp;
	while (flightList) {
           freePassengerList(flightList->passengerList);
           temp = flightList;
           flightList = flightList->next;
           free(temp);
       }

       return 0;
 }

/* End of file */




