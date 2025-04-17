#include "echo.h"
#include <arpa/inet.h>
#include <stdio.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char*argv[]) {
    parseInput(argc, argv);
    int server_socket = createSocket(DEFAULT_PORT);
    struct sockaddr_in client_addr;
    socklen_t client_len = sizeof(client_addr);
    int client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &client_len);

    while (1) {
        listen(server_socket, SOMAXCONN);
        if (client_socket < 0) {
            perror("accept failed");
            continue;
        }

        handleConnection(client_socket);
        close(client_socket);
    }
    return 0;
    
}