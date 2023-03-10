#include "udp_operations.h"
#include "operations.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>

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
    printf("Message sent: %s\n", msg);

    return n;
}

char* receiveFromUDP(int udpSocket, ssize_t *n, char* buffer){
    struct sockaddr addr;
    socklen_t addrlen;
    int errcode = -1;

    addrlen = sizeof(addr);
    *n = recvfrom(udpSocket, buffer, 128, 0, &addr, &addrlen);
    if (*n == -1){
        fprintf(stderr, "(recvfrom) ERROR: %s\n", gai_strerror(errcode));
    } else {
        buffer[*n] = '\0';
        printf("Message received:\n%s\n", buffer);
    }

    return  buffer;
}
