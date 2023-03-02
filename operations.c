#include "operations.h"
#include <string.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>


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
