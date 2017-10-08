//
//  main.cpp
//  Chat
//
//  Created by David Kopala on 10/7/17.
//  Copyright © 2017 David Kopala. All rights reserved.
//

#include <iostream>
#include <arpa/inet.h>

#include "UDPServer.h"
#include "UDPMessage.h"

using namespace std;

string printableIP(struct in_addr addr) {
    return inet_ntoa(addr);
}

void udpRead(UDPMessage *msg) {
    string ip = printableIP(msg->cliaddr->sin_addr);
    string console = ip + ": " + msg->message;
    cout<<console<<endl;
}

int main(int argc, const char * argv[]) {
    
}
