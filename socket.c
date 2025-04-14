#include "echo.h"
#include <arpa/inet.h>
#include <stdio.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>

/*
    Creates socket,
    1. creates socket with IPv4, and makes sure that connection exists before sending data
    2. Assigns newly created socket with (IP + port) [aka Bindng]
    3. Checks if Binding is valid
*/
int createSocket(int port) {
    int socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_fd < 0) {
        perror("Socket Creation Failed");
        return -1;
    }

    struct sockaddr_in socket_address;
    memset(&socket_address, '\0', sizeof(socket_address));
    socket_address.sin_family = AF_INET;
    socket_address.sin_addr.s_addr = htonl(INADDR_ANY);
    socket_address.sin_port = htons(port);

    printf("Binding to port %d\n", port);

    int returnval;

    returnval = bind(
        socket_fd, (struct sockaddr*)&socket_address, sizeof(socket_address));
    if (returnval < 0) {
        perror("bind");
        return 1;
    }
    return returnval;
}