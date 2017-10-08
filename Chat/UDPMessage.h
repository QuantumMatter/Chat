//
//  UDPMessage.h
//  Playground
//
//  Created by David Kopala on 8/13/17.
//  Copyright © 2017 David Kopala. All rights reserved.
//

#ifndef UDPMessage_h
#define UDPMessage_h

struct UDPMessage {
    char *message;
    struct sockaddr_in *cliaddr;
};

#endif /* UDPMessage_h */
