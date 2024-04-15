#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdint.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <openssl/sha.h>
#include "usage.h"
#include <math.h>

#define BUFFER_SIZE 4096

char* address = "bc1qcwty7znsn8djc92fdrmftyc7hwlqfc5g9ty7hx";
char* nonce;

int get(int i){
    if (i <= 10){
        return 0;
    }
    else{
        return round(i / 3);
    }
}

char* get_nonce() {
    uint32_t rand_num = rand();
    char* nonce_str = (char*)malloc(9 * sizeof(char));
    sprintf(nonce_str, "%08x", rand_num);
    return nonce_str;
}

void nonce_work(int sock, char* job_id, char* extranonce2, char* ntime) {
    nonce = get_nonce();
    char payload[BUFFER_SIZE];
    snprintf(payload, BUFFER_SIZE, "{\"params\": [\"%s\", \"%s\", \"%s\", \"%s\", \"%s\"], \"id\": 1, \"method\": \"mining.submit\"}\n", address, job_id, extranonce2, ntime, nonce);
    send(sock, payload, strlen(payload), 0);
    
    // Réception de la réponse du serveur
    char response[BUFFER_SIZE];
    ssize_t bytes_received = recv(sock, response, BUFFER_SIZE, 0);
    if (bytes_received < 0) {
        perror("Error receiving response");
        return;
    }
    
    // Affichage de la réponse reçue
    printf("Response from server: %.*s\n", (int)bytes_received, response);
    
    free(nonce);
}

int mine() {
    char* host = "solo.ckpool.org";
    int port = 3333;
    char* job_id;
    char* extranonce1;
    char* extranonce2;
    char* ntime;

    // Résolution DNS pour obtenir l'adresse IP du serveur
    struct hostent *server_info = gethostbyname(host);
    if (server_info == NULL) {
        perror("Host not found");
        return 1;
    }
    struct in_addr **addr_list = (struct in_addr **)server_info->h_addr_list;
    char *server_ip = inet_ntoa(*addr_list[0]);

    srand(time(NULL));
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_port = htons(port);
    server.sin_addr.s_addr = inet_addr(server_ip);

    if (connect(sock, (struct sockaddr*)&server, sizeof(server)) < 0) {
        perror("Connection failed");
        return 1;
    }

    char* request = "{\"id\": 1, \"method\": \"mining.subscribe\", \"params\": []}\n";
    send(sock, request, strlen(request), 0);

    char response[BUFFER_SIZE];
    recv(sock, response, BUFFER_SIZE, 0);
    sscanf(response, "{\"result\": [%s, %s, %s],", job_id, extranonce1, extranonce2);

    char* authorize_request = (char*)malloc(BUFFER_SIZE * sizeof(char));
    snprintf(authorize_request, BUFFER_SIZE, "{\"params\": [\"%s\", \"password\"], \"id\": 2, \"method\": \"mining.authorize\"}\n", address);
    send(sock, authorize_request, strlen(authorize_request), 0);
    free(authorize_request);

    char* notify_response = (char*)malloc(BUFFER_SIZE * sizeof(char));
    while (1) {
        recv(sock, notify_response, BUFFER_SIZE, 0);
        if (strstr(notify_response, "mining.notify") != NULL)
            break;
    }
    free(notify_response);

    for (int k = 0; k < 10000000; k++) {
        nonce_work(sock, job_id, extranonce2, ntime);
        sleep(get(cpu_usage()));
    }
    close(sock);
    return 0;
}
