#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>
#include "btree.h"

#define PORT 8080
#define BUFFER_SIZE 1024

BTreeNode *store;
FILE *aof_file;

void handle_command(char *cmd, char *response) {
    if (strcmp(cmd, "ping") == 0) {
        sprintf(response, "pong");
    }
    else if (strncmp(cmd, "SET ", 4) == 0) {
        char *key_str = strtok(cmd + 4, " ");
        char *value = strtok(NULL, "");
        int key = atoi(key_str);
        btree_insert(&store, key, value);
        fprintf(aof_file, "SET %d %s\n", key, value);
        fflush(aof_file);
        sprintf(response, "OK");
    }
    else if (strncmp(cmd, "GET ", 4) == 0) {
        char *key_str = cmd + 4;
        int key = atoi(key_str);
        char *value;
        if (btree_search(store, key, &value)) {
            sprintf(response, "%s", value);
        } else {
            sprintf(response, "(null)");
        }
    }
    else if (strncmp(cmd, "DELETE ", 7) == 0) {
        char *key_str = cmd + 7;
        int key = atoi(key_str);
        fprintf(aof_file, "DELETE %d\n", key);
        fflush(aof_file);
        sprintf(response, "DELETED");
    }
    else {
        sprintf(response, "UNKNOWN COMMAND");
    }
}

void *client_handler(void *arg) {
    int new_socket = *(int*)arg;
    free(arg);
    char buffer[BUFFER_SIZE] = {0};
    char response[BUFFER_SIZE] = {0};

    read(new_socket, buffer, BUFFER_SIZE);
    printf("Client: %s\n", buffer);
    handle_command(buffer, response);
    send(new_socket, response, strlen(response), 0);
    printf("Server: %s\n", response);
    close(new_socket);
    return NULL;
}


void start_cdfs_server() {
    
    char *video_memory = (char*) 0xb8000;
    char *msg = "CDFS Server Started from OS!";
    int i = 0;

    
    while (msg[i] != 0) {
        video_memory[(i+2) * 2] = msg[i];      
        video_memory[(i+2) * 2 + 1] = 0x02;     
        i++;
    }

    
    while(1);  
}

int main() {
    store = create_node(1);
    aof_file = fopen("cdfs.aof", "a+");
    if (!aof_file) {
        perror("Failed to open AOF file");
        exit(1);
    }

    int server_fd, new_socket;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);
    bind(server_fd, (struct sockaddr *)&address, sizeof(address));

    listen(server_fd, 5);
    printf(" CDFS Server (B-Tree) listening on port %d\n", PORT);

    while (1) {
        new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen);
        pthread_t thread;
        int *client_sock = malloc(sizeof(int));
        *client_sock = new_socket;
        pthread_create(&thread, NULL, client_handler, client_sock);
        pthread_detach(thread);
    }

    fclose(aof_file);
    return 0;
}