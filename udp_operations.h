#include <netdb.h>

#ifndef NETWORK_SIMULATION_UDP_OPERATIONS_H
#define NETWORK_SIMULATION_UDP_OPERATIONS_H

int createUDPSocket();
ssize_t sendToUDP(int udpSocket, char* msg, struct addrinfo* destAddr);
ssize_t receiveFromUDP(int udpSocket);

#endif //NETWORK_SIMULATION_UDP_OPERATIONS_H
