//===========================================
// Task 2 - PG3401 Exam                                       
// File Managment and Functions     
// Author: Candidate Number 3                                   
// Date: May 2025                                  
//===========================================

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "task2_hash.h"
#include "task2_sum.h"
#include "task2_count.h"
#include "task2_struct.h"


int main() {
       /* Åpner inputfil for lesing */
       FILE* fp = fopen("pgexam25_test.txt", "r");
       if (!fp) {
	   perror("Failed to open file");
	   return 1;
    }

     struct TASK2_FILE_METADATA meta = { 0 };
     strncpy(meta.szFileName, "pgexam25_test.txt", sizeof(meta.szFileName) - 1);

    /* Hash-funksjon */
    unsigned int hash = 0;
    Task2_SimpleDjb2Hash(fp, &hash);
    memcpy (meta.byHash, &hash, sizeof(meta.byHash));

    /* Finner størrelsen på filen og summerer alle karakterene */
    Task2_SizeAndSumOfCharacters(fp, &meta.iFileSize, &meta.iSumOfChars);

    /* Teller forekomst av bokstaver */
    Task2_CountEachCharacter(fp, meta.aAlphaCount);

    /* Lukker inputfilen */
    fclose(fp);

    /* Åpner outputfil for skriving av binærdata */
    FILE* out = fopen("pgexam25_output.bin", "wb");
    if (!out) {	
       perror("Failed to open file");
       return 1;
    }

    /* Skriver metadata-strukturen til outputfilen */
    fwrite(&meta, sizeof(struct TASK2_FILE_METADATA), 1, out);
    fclose(out); 	

     printf("Task completed\n");
     return 0;

}

/* End of file */
