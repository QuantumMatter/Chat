//
//  Server.hpp
//  Playground
//
//  Created by David Kopala on 7/23/17.
//  Copyright © 2017 David Kopala. All rights reserved.
//

#ifndef Server_h
#define Server_h

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>
#include <pthread.h>
#include <functional>

class Server
{
    
public:
    Server(char *port);
    void create();
    static void sigchld_handler(int s);
    static void *listening_handler(void *sockfd);
    static void *connection_handler(void *socket_desc);
    
    struct smsg {
        char *message;
        struct smsg *next;
    };
    struct socketList {
        int sockfd;
        struct socketList *next;
    };
    
    void writeToAll(char *message);
    static struct socketList *clients;
    
    static std::function<void(void)> newConnectionCallback;
    static std::function<void(smsg *msg)> newMessageCallback;
    static struct smsg *messages;
    
private:
    void *get_in_addr(struct sockaddr *sa);
};

#endif /* Server_hpp */
