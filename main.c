#include "operations.h"
#include "udp_operations.h"
#include "tcp_operations.h"
#include "interface.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

ssize_t leaveNetwork(int udpSocket, int tcpSocket, char* net, char* id, struct addrinfo* nodeServerAddr){
    char *buffer = (char*) malloc(sizeof(char) * 129);
    char unregCmd[13] = "";
    ssize_t n;

    //send UNREG commando to nodes server
    snprintf(unregCmd, sizeof unregCmd, "%s %s %s", "UNREG", net, id);
    n = sendToUDP(udpSocket, unregCmd, nodeServerAddr);
    if(n == -1) {return n;}
    buffer = receiveFromUDP(udpSocket, &n, buffer);

    //close TCP sockets
    close(tcpSocket);
    freeaddrinfo(nodeServerAddr);
    free(buffer);
    return n;
}

int exitProgram(int udpSocket, int tcpSocket, char* net, char* id, struct addrinfo* nodeServerAddr){
    char *buffer = (char*) malloc(sizeof(char) * 129);
    char unregCmd[13] = "";
    ssize_t n;

    snprintf(unregCmd, sizeof unregCmd, "%s %s %s", "UNREG", net, id);
    n = sendToUDP(udpSocket, unregCmd, nodeServerAddr);
    buffer = receiveFromUDP(udpSocket, &n, buffer);

    close(tcpSocket);
    close(udpSocket);
    freeaddrinfo(nodeServerAddr);
    free(buffer);
    exit(0);
}

int afterJoin(int udpSocket, int tcpSocket, char* net, char* id, struct addrinfo *nodeServerAddr){
    while (1){
        char *input = (char*) malloc(sizeof(char) * 128);
        char *cmd = (char*) malloc(sizeof(char) * 64);
        //Menu after joining a network
        printf("Input command:\n");
        printf("1.leave\n");
        printf("2.exit\n");

        //Get next command
        fgets(input, sizeof(input), stdin);
        sscanf(input, "%s", cmd);

        //Leave the network/Close node
        if(strcmp(cmd, "leave") == 0){
            leaveNetwork(udpSocket, tcpSocket, net, id, nodeServerAddr);
            free(cmd);
            free(input);
            break;
        } else if(strcmp(cmd, "exit") == 0){
            free(cmd);
            free(input);
            exitProgram(udpSocket, tcpSocket, net, id, nodeServerAddr);
        }
    }
}

int joinNetwork(char input[], int udpSocket, int errcode, char* port){
    char cmd[128];
    char net[4];
    char id[3];
    char *buffer = (char*) malloc(sizeof(char) * 129);

    sscanf(input,"%s %s %s", cmd, net, id);
    printf("Command: %s %s %s\n", cmd, net, id);

    //Create TCP socket
    int tcpSocket = createTCPSocket();
    if(tcpSocket == -1){ return tcpSocket;}

    //Initiate TCP Server
    errcode = initTCPServer(tcpSocket, port);
    if(errcode != 0){ return errcode;}

    //Get Node Server Address Info
    struct addrinfo *nodeServerAddr = getNodeServerAddrinfo(&errcode);
    if(errcode != 0){ return errcode;}

    //Get nodelist from node server's net
    char sendCmd[9] = "NODES ";
    strcat(sendCmd, net);
    ssize_t n;

    //Send "NODES <net> command to node server
    n = sendToUDP(udpSocket, sendCmd, nodeServerAddr);
    if(n == -1){ return (int) n;}

    //Receive NODESLIST <net>
    buffer = receiveFromUDP(udpSocket, &n, buffer);

    //add net's first node
    char regCmd[40] = "";

    //send REG command do nodes server
    snprintf(regCmd, sizeof regCmd, "%s %s %s %s %s", "REG", net, id, getHostIP(), port);
    n = sendToUDP(udpSocket, regCmd, nodeServerAddr);
    buffer = receiveFromUDP(udpSocket, &n, buffer);

    //If registration of node is success
    if(strcmp(buffer, "OKREG") == 0){
        printf("\nNode <%s> registered with success in network <%s>\n", id, net);

        //After joining network
        free(buffer);
        afterJoin(udpSocket, tcpSocket, net, id, nodeServerAddr);
    }

//        //net has more than 1 node
//        char *ptr = strtok(buffer, "\n");
//        while (ptr != NULL) {
//            ptr = strtok(NULL, "\n");
//            if (ptr != NULL) {
//                char idInServer[3];
//                memcpy(idInServer, ptr, 2);
//                idInServer[2] = '\0';
//                if(strcmp(idInServer, id) == 0){
//                    printf("ID (%s) already in use\n", id);
//                }
//            } else{break;}
//        }
        //add net's first node

}

int main() {
    char input[128];
    char cmd[128];
    int errcode = 0;
    char *port = "801";

    //Create UDP socket
    int udpSocket = createUDPSocket();
    if(udpSocket == -1){ exit(udpSocket);}

    while(1){
        printCommands();
        fgets(input, sizeof(input), stdin);
        sscanf(input, "%s", cmd);

        if(strcmp(cmd, "join") == 0 && strlen(input) == 12){
            joinNetwork(input, udpSocket, errcode, port);

        }
        else if(strcmp(cmd, "show") == 0){
            char net[4];
            char *buffer = (char*) malloc(sizeof(char) * 129);

            sscanf(input,"%s %s %s", cmd, cmd, net);
            printf("Command: %s %s\n", cmd, net);

            //Get Node Server Address Info
            struct addrinfo *res = getNodeServerAddrinfo(&errcode);
            if(errcode != 0){ continue;}

            //Get nodelist from node server's net
            char sendCmd[9] = "NODES ";
            strcat(sendCmd, net);
            ssize_t n;

            //Send "NODES <net> command to node server
            n = sendToUDP(udpSocket, sendCmd, res);
            if(n == -1){ continue;}

            //Receive NODESLIST <net>
            buffer = receiveFromUDP(udpSocket, &n, buffer);


            free(buffer);

        } else if(strcmp(cmd, "exit") == 0){
            close(udpSocket);
            exit(0);
        } else {
            printf("Invalid command\n");
        }
    }
}
