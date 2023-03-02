#include "operations.h"
#include "udp_operations.h"
#include "tcp_operations.h"
#include "interface.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

int main() {
    char input[128];
    char cmd[128];
    int errcode = 0;
    char *port = "801";
    while(1){
        printCommands();
        fgets(input, sizeof(input), stdin);
        sscanf(input, "%s", cmd);

        if(strcmp(cmd, "join") == 0 && strlen(input) == 12){
            char net[4];
            char id[3];
            char *buffer = (char*) malloc(sizeof(char) * 129);

            sscanf(input,"%s %s %s", cmd, net, id);
            printf("Command: %s %s %s\n", cmd, net, id);

            //Create TCP socket
            int tcpSocket = createTCPSocket();
            if(tcpSocket == -1){ continue;}

            //Initiate TCP Server
            errcode = initTCPServer(tcpSocket, port);
            if(errcode != 0){ continue;}

            //Create UDP socket
            int udpSocket = createUDPSocket();
            if(udpSocket == -1){ continue;}

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

            if(n > 14){
                //net has more than 1 node
                //n = sendToUDP(udpSocket, "UNREG 100 01", res);
            } else{
                //add net's first node
                char regCmd[40] = "";
                snprintf(regCmd, sizeof regCmd, "%s %s %s %s %s", "REG", net, id, getHostIP(), port);
                n = sendToUDP(udpSocket, regCmd, res);
                buffer = receiveFromUDP(udpSocket, &n, buffer);
                if(strcmp(buffer, "OKREG") == 0){
                    printf("\nNode <%s> registered with success in network <%s>\n", id, net);
                    while (1){
                        printf("Input command:\n");
                        printf("1.leave\n");
                        printf("2.exit\n");
                        fgets(input, sizeof(input), stdin);
                        sscanf(input, "%s", cmd);

                        if(strcmp(cmd, "leave") == 0){
                            char unregCmd[13] = "";
                            snprintf(unregCmd, sizeof unregCmd, "%s %s %s", "UNREG", net, id);
                            n = sendToUDP(udpSocket, unregCmd, res);
                            buffer = receiveFromUDP(udpSocket, &n, buffer);

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




        } else if(strcmp(cmd, "exit") == 0){
            exit(0);
        } else {
            printf("Invalid command\n");
        }
    }
}
