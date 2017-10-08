//
//  Client.hpp
//  Playground
//
//  Created by David Kopala on 7/23/17.
//  Copyright © 2017 David Kopala. All rights reserved.
//

#ifndef Client_h
#define Client_h

#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <functional>

#include <arpa/inet.h>

typedef struct node {
    char *message;
    struct node *next;
} node_t;

class Client
{
public:
    Client(char *hostname, int port);
    void send(char *message);
    
    node_t *getReceived();
    void writeMessage(char *message);
    
    static void *recvHandler(void *sock_desc);
    
    static std::function<void(node *msg)> readCallback;

private:
    int sock;
};

#endif /* Client_h */
