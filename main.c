#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>
#include "main.h"

typedef struct {
    int client_file_desc;
    int verbose;
} client_info_t;

void *handle_client(void *arg) {
    client_info_t *client_info = (client_info_t *) arg;
    int client_file_desc = client_info->client_file_desc;
    int verbose = client_info->verbose;
    char buffer[BUFFER_SIZE];

    ssize_t bytes_read;
    while ((bytes_read = recv(client_file_desc, buffer, BUFFER_SIZE - 1, 0)) > 0) {
        buffer[bytes_read] = '\0';
        if (verbose) {
            printf("Received: %s", buffer);
        }
        send(client_file_desc, buffer, bytes_read, 0);
    }

    if (bytes_read == 0) {
        printf("Client Disconnected. \n");
    } else if (bytes_read == -1) {
        perror("recv");
    }

    close(client_file_desc);
    free(client_info);
    pthread_exit(NULL);
}


int main(int argc, char *argv[]) {
    int port = DEFAULT_PORT;
    int verbose = 0;

    for (int index = 1; index < argc; index++) {
        if (strcmp(argv[index], "-p") == 0 && index + 1 < argc) {
            char *end_pointer;
            long port_long = strtol(argv[index + 1], &end_pointer, 10);

            if (*end_pointer != '\0' || port_long <= 0 || port_long > 65535) {
                fprintf(stderr, "Invalid port number.\n");
                exit(EXIT_FAILURE);
            }
            port = (int)port_long;
            index++;

        } else if (strcmp(argv[index], "-v") == 0) {
            verbose = 1;
        } else {
            fprintf(stderr, "Unknown option: %s\n", argv[index]);
            fprintf(stderr, "Usage: %s [-p port] [-v]\n", argv[0]);
            exit(EXIT_FAILURE);
        }
    }

    int server_file_desc;
    int client_file_desc;
    struct sockaddr_in server_addr, client_addr;
    char buffer[BUFFER_SIZE];
    socklen_t addr_len;

    // Create Socket
    if ((server_file_desc = socket(AF_INET, SOCK_STREAM, 0)) == -1 ) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    // Sever Address Setup
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    // Binding
    if (bind(server_file_desc, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("bind");
        close(server_file_desc);
        exit(EXIT_FAILURE);
    }

    // Start listening
    if (listen(server_file_desc, 1) == -1) {
        perror("listen");
        close(server_file_desc);
        exit(EXIT_FAILURE);
    }

    printf("Echo server listening on port %d...\n", port);

    while (1) {
        addr_len = sizeof(client_addr);
        client_file_desc = accept(server_file_desc, (struct sockaddr *)&client_addr, &addr_len);
        if (client_file_desc == -1) {
            perror("accept");
            continue;
        }

        printf("Client connected.\n");

        client_info_t *client_info = malloc(sizeof(client_info_t));
        if (client_info == NULL) {
            perror("malloc");
            close(client_file_desc);
            continue;
        }
        
        client_info->client_file_desc = client_file_desc;
        client_info->verbose = verbose;

        pthread_t thread_id;
        if (pthread_create(&thread_id, NULL, handle_client, (void *)client_info) != 0) {
            perror("pthread create");
            close(client_file_desc);
            free(client_info);
            continue;;
        }

        pthread_detach(thread_id);
    }
    close(server_file_desc);
    return 0;
}