#include <netdb.h>

#ifndef NETWORK_SIMULATION_UDP_OPERATIONS_H
#define NETWORK_SIMULATION_UDP_OPERATIONS_H

int createUDPSocket();
ssize_t sendToUDP(int udpSocket, char* msg, struct addrinfo* destAddr);
char* receiveFromUDP(int udpSocket, ssize_t *n, char* buffer);

#endif //NETWORK_SIMULATION_UDP_OPERATIONS_H
