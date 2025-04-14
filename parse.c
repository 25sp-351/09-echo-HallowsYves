#include "echo.h"
#include "string.h"
#include <stdio.h>
#include <stdlib.h>

void parseInput(int argc, char *argv[]) {
    for (int index = 1; index < argc; index++) {
        if (strcmp(argv[index], "-p") == 0 && index + 1 < argc) {

            // String to Long conversion
            char *end_pointer;
            long port_long = strtol(argv[index + 1], &end_pointer, 10);
            if (*end_pointer != '\0') {
                fprintf(stderr, "Invalid Port: %s\n", argv[index + 1]);
                exit(1);
            }

            DEFAULT_PORT = (int) port_long;
            index++;
        } else if (strcmp(argv[index], "-v") == 0) {
            VERBOSE = 1;
        }

    }
}