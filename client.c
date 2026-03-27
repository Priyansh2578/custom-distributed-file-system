#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFFER_SIZE 1024

int main() {
    int sock = 0;
    struct sockaddr_in serv_addr;
    char buffer[BUFFER_SIZE] = {0};

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        printf("Socket creation failed\n");
        return -1;
    }

    serv_addr.sin_family = AF_INET;  
    serv_addr.sin_port = htons(PORT);

    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        printf("Invalid address\n");
        return -1;
    }

    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        printf("Connection failed\n");
        return -1;
    }
    printf(" Connected to cdfs server\n");

    char *commands[] = {
    "ping",
    "SET 101 bhai",
    "GET 101",
    "SET 102 delhi",
    "GET 102",
    "GET 999",
    "DELETE 101",
    "GET 101",
    "EXIT"
    };
    
    for (int i = 0; i < 9; i++) {
        memset(buffer, 0, BUFFER_SIZE);
        
        send(sock, commands[i], strlen(commands[i]), 0);
        printf(" Client: %s\n", commands[i]);

        read(sock, buffer, BUFFER_SIZE);
        printf(" Server: %s\n", buffer);
    }

    close(sock);
    printf(" Connection closed\n");

    return 0;
}