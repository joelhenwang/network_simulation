#include <netdb.h>

#ifndef NETWORK_SIMULATION_OPERATIONS_H
#define NETWORK_SIMULATION_OPERATIONS_H

struct addrinfo* getNodeServerAddrinfo(int* errcode);
char* getHostIP();


#endif //NETWORK_SIMULATION_OPERATIONS_H
