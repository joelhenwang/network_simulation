#include <sys/socket.h>
#include <stdio.h>
#include <netdb.h>
#include <string.h>
#include "tcp_operations.h"

int createTCPSocket(){
    int tcp, errcode;
    tcp = socket(AF_INET,SOCK_STREAM,0);
    if(tcp == -1){
        fprintf(stderr, "(socket (TCP)) ERROR: %s\n", gai_strerror(errcode));
    }

    return tcp;
}

int initTCPServer(int tcpSocket, char* port){
    struct addrinfo hints, *res;
    int errcode;


    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    errcode = getaddrinfo(NULL, port, &hints, &res);
    if(errcode != 0){
        fprintf(stderr, "(getaddrinfo) ERROR: %s\n", gai_strerror(errcode));
        return errcode;
    }

    errcode = bind(tcpSocket, res->ai_addr, res->ai_addrlen);
    if(errcode != 0){
        fprintf(stderr, "(bind) ERROR: %s\n", gai_strerror(errcode));
        return errcode;
    }

    errcode = listen(tcpSocket, 100);
    if(errcode != 0){
        fprintf(stderr, "(bind) ERROR: %s\n", gai_strerror(errcode));
        return errcode;
    }


}

