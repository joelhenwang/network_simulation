#include <netdb.h>

#ifndef NETWORK_SIMULATION_OPERATIONS_H
#define NETWORK_SIMULATION_OPERATIONS_H

struct addrinfo* getNodeServerAddrinfo(int* errcode);
int createUDPSocket();
ssize_t sendToUDP(int udpSocket, char* msg, struct addrinfo* destAddr);
ssize_t receiveFromUDP(int udpSocket);
char* getHostIP();


#endif //NETWORK_SIMULATION_OPERATIONS_H
