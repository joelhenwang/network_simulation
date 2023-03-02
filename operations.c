#include "operations.h"
#include <string.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>


struct addrinfo* getNodeServerAddrinfo(int* errcode){
    struct addrinfo hints, *res;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_DGRAM;
    hints.ai_flags = AI_CANONNAME;

    *errcode = getaddrinfo("193.136.138.142", "59000", &hints,&res);
    if(*errcode != 0){
        fprintf(stderr, "(getaddrinfo) ERROR: %s\n", gai_strerror(*errcode));
    }


    return res;
}

int createUDPSocket(){
    int udp, errcode;
    udp = socket(AF_INET,SOCK_DGRAM,0);
    if(udp == -1){
        fprintf(stderr, "(socket (UDP)) ERROR: %s\n", gai_strerror(errcode));
    }

    return udp;
}

ssize_t sendToUDP(int udpSocket, char* msg, struct addrinfo* destAddr){
    ssize_t n = -1;
    size_t msgLength = strlen(msg);
    int errcode = -1;

    n = sendto(udpSocket, msg, msgLength, 0,destAddr->ai_addr, destAddr->ai_addrlen);
    if (n == -1){
        fprintf(stderr, "(sendto) ERROR: %s\n", gai_strerror(errcode));
    }

    return n;
}

ssize_t receiveFromUDP(int udpSocket){
    struct sockaddr addr;
    socklen_t addrlen;
    ssize_t n = -1;
    char buffer[129];
    int errcode = -1;

    addrlen = sizeof(addr);
    n = recvfrom(udpSocket, buffer, 128, 0, &addr, &addrlen);
    if (n == -1){
        fprintf(stderr, "(recvfrom) ERROR: %s\n", gai_strerror(errcode));
    } else {
        buffer[n] = '\0';
        printf("Message received: %s\n", buffer);
    }

    return  n;
}

char* getHostIP(){
    char buffer[128], *ip;
    struct hostent *host_entry;
    int errcode;

    int hostname = gethostname(buffer, sizeof buffer);
    if(hostname == -1){
        fprintf(stderr, "(gethostname) ERROR: %s\n", gai_strerror(errcode));
    }

    host_entry = gethostbyname(buffer);
    if(host_entry == NULL){
        fprintf(stderr, "(gethostbyname) ERROR: %s\n", gai_strerror(errcode));
    }

    ip = inet_ntoa(*((struct in_addr*) host_entry->h_addr_list[0]));
    if(ip == NULL){
        fprintf(stderr, "(inet_ntoa) ERROR: %s\n", gai_strerror(errcode));
    }

    return ip;
}
