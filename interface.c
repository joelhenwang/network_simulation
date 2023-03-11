#include "operations.h"
#include "udp_operations.h"
#include "tcp_operations.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#include "interface.h"

void printAfterJoinCommands(){
    printf("Input command:\n");
    printf("1.leave\n");
    printf("2.exit\n");
}



char* initRegCommand(int udpSocket, char* net, char* id, char* port, ssize_t* n, struct addrinfo* res, char* buffer){
    char regCmd[40] = "";
    snprintf(regCmd, sizeof regCmd, "%s %s %s %s %s", "REG", net, id, getHostIP(), port);
    *n = sendToUDP(udpSocket, regCmd, res);

    if(*n == -1){return "error";}

    buffer = receiveFromUDP(udpSocket, n, buffer);

    return buffer;
}

char* initUnRegCommand(int udpSocket, char* net, char* id, ssize_t* n, struct addrinfo* res, char* buffer){
    char unregCmd[13] = "";
    snprintf(unregCmd, sizeof unregCmd, "%s %s %s", "UNREG", net, id);
    *n = sendToUDP(udpSocket, unregCmd, res);
    if(*n == -1){return "error";}
    buffer = receiveFromUDP(udpSocket, n, buffer);

    return buffer;
}

void leaveCommand(int udpSocket, int tcpSocket, char* net, char* id, ssize_t* n, struct addrinfo* res, char* buffer){
    initUnRegCommand(udpSocket, net, id, n, res, buffer);
    close(tcpSocket);
    close(udpSocket);
    freeaddrinfo(res);
    free(buffer);
}

void printCommands(){
    printf("Input command:\n");
    printf("\t1. join\n");
    printf("\t2. show NODES <net>\n");
    printf("\t2. exit\n");
    printf("\nInput:");
}

int initJoinCommand(char* input, char* port){
    int errcode;
    char net[4];
    char id[3];
    char cmd[128];
    char *buffer = (char*) malloc(sizeof(char) * 129);

    sscanf(input,"%s %s %s", cmd, net, id);
    printf("Command: %s %s %s\n", cmd, net, id);

    //Create TCP socket
    int tcpSocket = createTCPSocket();
    if(tcpSocket == -1){ return -1;}

    //Initiate TCP Server
    errcode = initTCPServer(tcpSocket, port);
    if(errcode != 0){ return -1;}

    //Create UDP socket
    int udpSocket = createUDPSocket();
    if(udpSocket == -1){ return -1;}

    //Get Node Server Address Info
    struct addrinfo *res = getNodeServerAddrinfo(&errcode);
    if(errcode != 0){ return -1;}

    //Get nodelist from node server's net
    char sendCmd[9] = "NODES ";
    strcat(sendCmd, net);
    ssize_t n;

    //Send "NODES <net> command to node server
    n = sendToUDP(udpSocket, sendCmd, res);
    if(n == -1){ return -1;}

    //Receive NODESLIST <net>
    buffer = receiveFromUDP(udpSocket, &n, buffer);

    if(n > 14){
        //net has more than 1 node
        //get list of nodes in <net>
        //check if <ip>:<port> exists
        //check if <id> exists
        //n = sendToUDP(udpSocket, "UNREG 100 01", res);
    } else{
        //add net's first node
        buffer = initRegCommand(udpSocket, net, id, port, &n, res, buffer);

        if(strcmp(buffer, "OKREG") == 0){
            printf("\nNode <%s> registered with success in network <%s>\n", id, net);
            while (1){
                printAfterJoinCommands();
                printf("Input command:\n");

                //Wait and get input
                fgets(input, sizeof(input), stdin);
                sscanf(input, "%s", cmd);

                if(strcmp(cmd, "leave") == 0){
                    initUnRegCommand(udpSocket, net, id, &n, res, buffer);

                    close(tcpSocket);
                    close(udpSocket);
                    freeaddrinfo(res);
                    free(buffer);
                    break;
                } else if(strcmp(cmd, "exit") == 0){
                    char unregCmd[13] = "";
                    snprintf(unregCmd, sizeof unregCmd, "%s %s %s", "UNREG", net, id);
                    n = sendToUDP(udpSocket, unregCmd, res);
                    buffer = receiveFromUDP(udpSocket, &n, buffer);

                    close(tcpSocket);
                    close(udpSocket);
                    freeaddrinfo(res);
                    free(buffer);
                    exit(0);
                }
            }
        }

    }
}
