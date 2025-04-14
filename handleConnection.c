#include "echo.h"
#include <arpa/inet.h>
#include <stdio.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>


void handleConnection(int a_client) {
    int socket_fd = createSocket(DEFAULT_PORT);

    // Read input 
    char buffer[1024];
    ssize_t bytes_read = read(socket_fd, buffer, sizeof(buffer));
    char temp_buffer[1024];
    int line_index = 0;

    
    // Loop through the buffer to see where "\n" is
    for (int index = 0; index < bytes_read; index++) {


        // Store values in temp_buffer 
        temp_buffer[line_index++] = buffer[index];

        if (buffer[index] == '\n') {
            // Once we get '\n' we send it back to the client, and reset line Index, 
            temp_buffer[0] = buffer[index];
        }

    }

}