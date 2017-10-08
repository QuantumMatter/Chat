//
//  UDPServer.cpp
//  Playground
//
//  Created by David Kopala on 8/13/17.
//  Copyright © 2017 David Kopala. All rights reserved.
//

#include "UDPServer.h"

#define BUFSIZE 2048

std::function<void(UDPMessage*)> UDPServer::readCallback;
int UDPServer::p;
List<UDPMessage> *UDPServer::messages;
UDPServer::UDPServer(int port)
{
    struct sockaddr_in myaddr;
    
    p = port;
    
    //Create UDP Socket
    if ((fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("Counld not create socket");
    }
        
    //Name UDP Socket
    //Bind to any client
    memset((char *)&myaddr, 0, sizeof(myaddr));
    myaddr.sin_family = AF_INET;
    myaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    myaddr.sin_port = htons(port);
    
    if (bind(fd, (struct sockaddr *)&myaddr, sizeof(myaddr)) < 0) {
        perror("Could not bind socket");
    }
    
    //Set up broadcasting sockaddr
    memset((char *)&cliaddr, 0, sizeof(cliaddr));
    cliaddr.sin_family = AF_INET;
    cliaddr.sin_port = htons(port);
    
    //Create message list
    messages = new List<UDPMessage>();
    
    //Set up receiving thread
    pthread_t recvThread;
    int *recvFd = (int *)malloc(1);
    *recvFd = fd;
    if (pthread_create(&recvThread, NULL, UDPServer::listening_handler, (void *)recvFd) < 0) {
        perror("Could not create receiving handler");
    }
}

void *UDPServer::listening_handler(void *sock)
{
    sockaddr_in baddr;
    socklen_t bSize = sizeof(baddr);
    
    int mysock = *(int*)sock;
    char message[BUFSIZE];
    
    while (true) {
        recvfrom(mysock, &message, BUFSIZE, 0, (struct sockaddr *)&baddr, &bSize);
        UDPMessage *msg = (UDPMessage *)malloc(sizeof(UDPMessage));
        msg->message = (char *)malloc(BUFSIZE);
        msg->cliaddr = (struct sockaddr_in *) malloc(sizeof(sockaddr_in));
        memcpy(msg->cliaddr, &baddr, bSize);
        strcpy(msg->message, message);
        messages->add(msg);
        if (readCallback != NULL) {
            readCallback(msg);
        }
    }
    return 0;
}

void UDPServer::broadcast(char *message)
{
    sendto(fd, message, strlen(message), 0, (sockaddr *)&cliaddr, sizeof(cliaddr));
}
