//
//  UDPServer.hpp
//  Playground
//
//  Created by David Kopala on 8/13/17.
//  Copyright © 2017 David Kopala. All rights reserved.
//

#ifndef UDPServer_h
#define UDPServer_h

#include <stdio.h>
#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <ifaddrs.h>
#include <string>
#include <cstring>
#include <pthread.h>
#include <functional>

#include "UDPMessage.h"

#include "List.h"

class UDPServer {

private:
    int fd;
    struct sockaddr_in cliaddr;
    static int p;//port

public:
    UDPServer(int port);

    void broadcast(char *message);
    static void *listening_handler(void *test);

    static List<UDPMessage> *messages;
    static std::function<void(UDPMessage *message)> readCallback;
};

#endif /* UDPServer_h */
