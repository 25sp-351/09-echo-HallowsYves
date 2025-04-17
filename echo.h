#ifndef ECHO_H
#define ECHO_H


int DEFAULT_PORT;
int VERBOSE;
void handleConnection(int a_client);
int createSocket(int port);
void parseInput(int argc, char *argv[]);


#endif