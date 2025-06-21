//===========================================
// Task 4 - PG3401 Exam
// Threads  
// Author: Candidate Number 3
// Date: May 2025
//===========================================

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <semaphore.h>

#define BUFFER_SIZE 4096
#define NUM_THREADS 2
#define BYTE_RANGE 256
#define BLOCK_SIZE 8

/* Funksjonserklæringer – implementasjon er i tea.c og dbj2.c */
void encipher(unsigned int *const v, unsigned int *const w, const unsigned int *const k);
int Task2_SimpleDjb2Hash(FILE *f, int *piHash);

/* Struktur for å dele data mellom trådene */
typedef struct {
    unsigned char *pBuffer;
    int bytesInBuffer;
    int done;
    char filename[256];
    pthread_mutex_t mutex;
    sem_t semEmpty;
    sem_t semFull;
} SHARED_DATA;


//==========================================
// Tråd A - Leser data fra fil inn i buffer
//==========================================
void *threadA(void *arg) {
    SHARED_DATA *pData = (SHARED_DATA *)arg;
    FILE *fp = fopen(pData->filename, "rb");
    if (!fp) {
        perror("Failed to open file");
        pthread_exit(NULL);
    }

    while (1) {
        sem_wait(&pData->semEmpty);
        pthread_mutex_lock(&pData->mutex);
        pData->bytesInBuffer = fread(pData->pBuffer, 1, BUFFER_SIZE, fp);
        if (pData->bytesInBuffer < BUFFER_SIZE) {
            pData->done = 1;
        }
        pthread_mutex_unlock(&pData->mutex);
        sem_post(&pData->semFull);
        if (pData->done) break;
    }

    fclose(fp);
    pthread_exit(NULL);
}


//=================================================================
// Tråd B - Leser fra buffer, hasher og krypterer, skriver til .enc
//=================================================================
void *threadB(void *arg) {
    SHARED_DATA *pData = (SHARED_DATA *)arg;
    FILE *out = fopen("task4_pg2265.enc", "wb");
    if (!out) {
        perror("Failed to ope output file");
        pthread_exit(NULL);
    }

    int totalHash = 5381; /* startverdi for djb2 hash */
    unsigned int key[4] = {1, 2, 3, 4};  
    unsigned char paddedBlock[BLOCK_SIZE];

    while (1) {
        sem_wait(&pData->semFull);
        pthread_mutex_lock(&pData->mutex);

        int len = pData->bytesInBuffer;
        int done = pData->done;
        unsigned char *buf = pData->pBuffer;

        pthread_mutex_unlock(&pData->mutex);
        sem_post(&pData->semEmpty);

        /* Beregn hash */
        for (int i = 0; i < len; i++) {
            totalHash = ((totalHash << 5) + totalHash) + buf[i];
        }

        /* Krypter i blokker på 8 byte */
        int i = 0;
        while (i + 8 <= len) {
            encipher((unsigned int*)(buf + i), (unsigned int*)(buf + i), key);
            fwrite(buf + i, 1, 8, out);
            i += 8;
        }

        /* legg til padding og krypter */
        if (done) {
            int remaining = len - i;
            memcpy(paddedBlock, buf + i, remaining);
            int pad = 8 - remaining;
            for (int j = remaining; j < 8; j++) {
                paddedBlock[j] = pad;
            }
            encipher((unsigned int*)paddedBlock, (unsigned int*)paddedBlock, key);
            fwrite(paddedBlock, 1, 8, out);
            break;
        }
    }

    fclose(out);

    /* Skriv hash til fil */
    FILE *hf = fopen("task4_pg2265.hash", "w");
    if (hf) {
        fprintf(hf, "%d\n", totalHash);
        fclose(hf);
    }
    pthread_exit(NULL);
}

//===================================
// Main, starter tråder og rydder opp
//==================================
int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <inputfile>\n", argv[0]);
        return 1;
    }

    SHARED_DATA *pData = malloc(sizeof(SHARED_DATA));
    pData->pBuffer = malloc(BUFFER_SIZE);
    strncpy(pData->filename, argv[1], 255);
    pData->filename[255] = '\0';
    pData->bytesInBuffer = 0;
    pData->done = 0;

    pthread_mutex_init(&pData->mutex, NULL);
    sem_init(&pData->semEmpty, 0, 1);
    sem_init(&pData->semFull, 0, 0);

    /* Oppretter tråder */
    pthread_t tA, tB;
    pthread_create(&tA, NULL, threadA, pData);
    pthread_create(&tB, NULL, threadB, pData);

    /* Venter på trådene */
    pthread_join(tA, NULL);
    pthread_join(tB, NULL);

    /* Rydder opp */
    pthread_mutex_destroy(&pData->mutex);
    sem_destroy(&pData->semEmpty);
    sem_destroy(&pData->semFull);
    free(pData->pBuffer);
    free(pData);

    printf("Encryption and hash successfully written :)\n");
    return 0;
}

/* End of file */

