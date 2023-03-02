#include "operations.h"
#include "udp_operations.h"
#include "tcp_operations.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main() {
    char input[128];
    char cmd[128];
    while(printf("Input command:\n")){
        printf("\t1. join\n");
        printf("\t2. leave\n");
        printf("\t3. exit\n");
        printf("\nInput:");
        fgets(input, sizeof(input), stdin);
        sscanf(input, "%s", cmd);
        if(strcmp(cmd, "join") == 0){
            char net[4];
            char id[3];
            sscanf(input,"%s %s %s", cmd, net, id);
            printf("Command: %s %s %s\n", cmd, net, id);

            //create tcp server
            int tcp = createTCPSocket();
            int errcode = initTCPServer(tcp, "801");
            //show nodeslist
            //reg tcp
            //check if OKREG

        } else if(strcmp(cmd, "leave") == 0){
            //leave
        } else if(strcmp(cmd, "exit") == 0){
            //leave
            //exit
        } else {
            printf("Invalid command\n");
        }
    }


    char *ip;
    ip = getHostIP();
    if(ip != NULL){
        printf("Host IP: %s\n", ip);
    }


    int code = -1;
    struct addrinfo *res = getNodeServerAddrinfo(&code);
    if(code == 0){
        printf("Canonical name: %s\n", res->ai_canonname);
    }

    //------------------------------------------------------------

//    int udp = createUDPSocket();
//    if(udp != -1){
//        ssize_t n;
//        char msg[128];
//        while(printf("Type message to send: ") && fgets(msg, sizeof(msg), stdin) != 0) {
//            printf("\nMessage sent: %s", msg);
//            n = sendToUDP(udp, msg, res);
//            n = receiveFromUDP(udp);
//            msg[0] = '\0';
//        }
//
//    }

    freeaddrinfo(res);
    return 0;
}
