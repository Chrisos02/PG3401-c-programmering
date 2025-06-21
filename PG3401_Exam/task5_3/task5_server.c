//===========================================
// Task 5 - PG3401 Exam
// Network
// Author: Candidate Number 3
// Date: May 2025
//===========================================


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "include/ewpdef.h"

#define BUFFER_SIZE 1024

/* SERVERREPLY - sender en serverreply til klienten */
void sendReply(int fd, const char *code, const char *msg) {
     struct EWA_EXAM25_TASK5_PROTOCOL_SERVERREPLY reply = {0};
     strcpy(reply.stHead.acMagicNumber, "EWP");
     strcpy(reply.stHead.acDataSize, "0064");
     reply.stHead.acDelimeter[0] = '|';
     strcpy(reply.acStatusCode, code);
     reply.acHardSpace[0] = ' ';
     snprintf(reply.acFormattedString, sizeof(reply.acFormattedString), "%s", msg);
     reply.acHardZero[0] = 0;
     send(fd, &reply, sizeof(reply), 0);
}

int main(int argc, char *argv[]) {
     int port = 2500;
     char serverID[64] = "SmtpTest";

    /* Leser Port og serverID fra kommandolinjen */
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-port") == 0 && i + 1 < argc) {
            port = atoi(argv[++i]);
        } else if (strcmp(argv[i], "-id") == 0 && i + 1 < argc) {
            strncpy(serverID, argv[++i], sizeof(serverID) - 1);
        }
    }

    /* TCP socket setup */
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) { perror("socket"); exit(1); }


     /* Konfiguerer serveradresse */
     struct sockaddr_in server_addr = {0};
     server_addr.sin_family = AF_INET;
     server_addr.sin_port = htons(port);
     server_addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);

    /* Binder socket til adresse og port */
    if (bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("bind"); 
	close(server_fd); exit(1);
    }

    /* starter lytting etter tilkobling */
    if (listen(server_fd, 5) < 0) {
        perror("listen"); close(server_fd); exit(1);
    }

    printf("Server is running on port %d with ID '%s'\n", port, serverID);

    socklen_t client_len = sizeof(struct sockaddr_in);
    struct sockaddr_in client_addr;
    int client_fd = accept(server_fd, (struct sockaddr*)&client_addr, &client_len);
    if (client_fd < 0) { perror("accept"); close(server_fd); exit(1); }

    /* sender SERVERACCEPT melding med dato */
    struct EWA_EXAM25_TASK5_PROTOCOL_SERVERACCEPT acceptMsg = {0};
    strcpy(acceptMsg.stHead.acMagicNumber, "EWP");
    strcpy(acceptMsg.stHead.acDataSize, "0064");
    acceptMsg.stHead.acDelimeter[0] = '|';
    strcpy(acceptMsg.acStatusCode, "220");
    acceptMsg.acHardSpace[0] = ' ';

    /* Dato */
    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    snprintf(acceptMsg.acFormattedString, sizeof(acceptMsg.acFormattedString),
         "127.0.0.1 SMTP %s %02d May %d",serverID,t->tm_mday,t->tm_year + 1900);
    acceptMsg.acHardZero[0] = 0;
    send(client_fd, &acceptMsg, sizeof(acceptMsg), 0);


    /* CLIENTHELO - Mottar HELO og svarer med SERVERHELO */
    struct EWA_EXAM25_TASK5_PROTOCOL_CLIENTHELO heloMsg = {0};
    recv(client_fd, &heloMsg, sizeof(heloMsg), 0);
    printf("Received HELO: %s\n", heloMsg.acFormattedString);


     /* SERVERHELO */
     struct EWA_EXAM25_TASK5_PROTOCOL_SERVERHELO heloReply = {0};
     strcpy(heloReply.stHead.acMagicNumber, "EWP");
     strcpy(heloReply.stHead.acDataSize, "0064");
     heloReply.stHead.acDelimeter[0] = '|';
     strcpy(heloReply.acStatusCode, "250");
     heloReply.acHardSpace[0] = ' ';
     snprintf(heloReply.acFormattedString, sizeof(heloReply.acFormattedString), "%s Hello %s",
             inet_ntoa(client_addr.sin_addr), heloMsg.acFormattedString);
     heloReply.acHardZero[0] = 0;
     send(client_fd, &heloReply, sizeof(heloReply), 0);

     /* Leser MAIL FROM og svarer med 250 (ok) */
     struct EWA_EXAM25_TASK5_PROTOCOL_MAILFROM mailFrom = {0};
     recv(client_fd, &mailFrom, sizeof(mailFrom), 0);
     printf("Received MAIL FROM: %s\n", mailFrom.acFormattedString);
     sendReply(client_fd, "250", "Sender address ok");


     /* Leser RCPT TO og svarer med 250 */
     struct EWA_EXAM25_TASK5_PROTOCOL_RCPTTO rcptTo = {0};
     recv(client_fd, &rcptTo, sizeof(rcptTo), 0);
     printf("Received RCPT TO: %s\n", rcptTo.acFormattedString);
     sendReply(client_fd, "250", "Receiver address ok");


     /* Leser DATA */
     struct EWA_EXAM25_TASK5_PROTOCOL_CLIENTDATACMD dataCmd = {0};
     recv(client_fd, &dataCmd, sizeof(dataCmd), 0);
     printf("Received DATA file: %s\n", dataCmd.acFormattedString);


    /* Verifiserer at filen slutter med .eml */
    if (!strstr(dataCmd.acFormattedString, ".eml")) {
        sendReply(client_fd, "500", "Invalid filename");
        close(client_fd); close(server_fd); return 1;
    }
    sendReply(client_fd, "354", "Ready for message");

    /* Mottak av meldingsinnhold */
    char fileBuffer[BUFFER_SIZE] = {0};
    size_t offset = 0;
    FILE *outFile = fopen(dataCmd.acFormattedString, "wb");
    if (!outFile) { perror("fopen"); close(client_fd); close(server_fd); return 1; }

    /* Skriver FROM / TO i fil */
    fprintf(outFile, "From: %s\r\nTo: %s\r\n\r\n",
            mailFrom.acFormattedString, rcptTo.acFormattedString);

    /* Leser meldingsdata */
    int done = 0;
    while (!done) {
        struct EWA_EXAM25_TASK5_PROTOCOL_SIZEHEADER stHead;
        if (recv(client_fd, &stHead, sizeof(stHead), 0) <= 0) break;
        int payloadSize = atoi(stHead.acDataSize);
        if (payloadSize <= 0 || offset + payloadSize > BUFFER_SIZE) break;

        
        ssize_t total = 0;
        while (total < payloadSize) {
            ssize_t r = recv(client_fd, fileBuffer + offset + total, payloadSize - total, 0);
            if (r <= 0) { done = 1; break; }
            total += r;
        }
        offset += total;
        if (offset >= 5 && (strstr(fileBuffer, "\r\n.\r\n") || strstr(fileBuffer, "\n.\n"))) done = 1;
    }

     /* Skriver data til fil */
     char *end = strstr(fileBuffer, "\r\n.\r\n");
     if (!end) end = strstr(fileBuffer, "\n.\n");
     size_t toWrite = end ? (size_t)(end - fileBuffer) : offset;
     fwrite(fileBuffer, 1, toWrite, outFile);
     fclose(outFile);
     sendReply(client_fd, "250", "Message accepted");


     /* Leser og svarer på QUIT */
     struct EWA_EXAM25_TASK5_PROTOCOL_CLOSECOMMAND quitMsg = {0};
     recv(client_fd, &quitMsg, sizeof(quitMsg), 0);
     sendReply(client_fd, "221", "Server closed");

     /* Rydder opp og avslutter */
     close(client_fd);
     close(server_fd);

    return 0;
}

/* End of file */

