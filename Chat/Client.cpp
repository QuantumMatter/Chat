//
//  Client.cpp
//  Playground
//
//  Created by David Kopala on 7/23/17.
//  Copyright © 2017 David Kopala. All rights reserved.
//

#include "Client.h"

#define PORT "12345"
#define MAXDATASIZE 100

static node_t *messages;
std::function<void(node *msg)> Client::readCallback;

Client::Client(char *hostname, int port)
{
    node_t *lastMessage;
    struct sockaddr_in server;
    int *new_sock;
    
    messages = (node_t *) malloc(sizeof(node_t));
    messages->message = "First";
    messages->next = NULL;
    lastMessage = messages;
    
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1) {
        printf("Could not create socket\n");
        return;
    }
    
    server.sin_addr.s_addr = inet_addr(hostname);
    server.sin_family = AF_INET;
    server.sin_port = htons(port);
    
    if (connect(sock, (struct sockaddr *)&server, sizeof(server)) < 0) {
        printf("Connection Failed\n");
        return;
    }
    
    puts("Connected");
    
    //pthread_t sendThread;
    pthread_t recvThread;
    
    new_sock = (int *) malloc(1);
    *new_sock = sock;
    if (pthread_create(&recvThread, NULL, Client::recvHandler, (void *)new_sock) < 0) {
        
    }
}

node_t* Client::getReceived()
{
    return messages;
}

void Client::writeMessage(char *message)
{
    ssize_t e;
    if ((e = write(sock, message, strlen(message))) == -1) {
        perror("write error");
        //std::cout << std::strerror(errno) << '\n';
    }
}

void *Client::recvHandler(void *sock_desc)
{
    char message[500];
    int mysock = *(int*)sock_desc;
    
    while (true) {
        if (recv(mysock, message, 500, 0) > 0) {
            node_t *last = messages;
            while (last->next != NULL) {
                last = last->next;
            }
            last->next = (node_t *) malloc(sizeof(node_t));
            char mes_cpy[500] = {'\0', };
            strcpy(mes_cpy, message);
            last->next->message = mes_cpy;
            last->next->next = NULL;
            if (readCallback != NULL) {
                readCallback(last->next);
            }
            bzero(message, 500);
        }
    }
    return 0;
}
