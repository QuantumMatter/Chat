//
//  Server.cpp
//  Playground
//
//  Created by David Kopala on 7/23/17.
//  Copyright © 2017 David Kopala. All rights reserved.
//

#include "Server.h"

#define PORT "12345"
#define BACKLOG 10

Server::smsg *Server::messages;
Server::socketList *Server::clients;
std::function<void(void)> Server::newConnectionCallback;
std::function<void(Server::smsg *msg)> Server::newMessageCallback;

Server::Server(char *port)
{
    int rv;
    struct addrinfo hints, *server_info, *p;
    int sockfd;
    struct sigaction sa;
    
    int yes = 1;
    
    messages = (smsg *) malloc(sizeof(smsg));
    messages->message = "First";
    messages->next = NULL;
    
    clients = (socketList *)malloc(sizeof(socketList));
    clients->sockfd = -1;
    clients->next = NULL;
    
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;
    
    if ((rv = getaddrinfo(nullptr, port, &hints, &server_info)) != 0) {
        printf("ERROR A\n");
        return;
    }
    
    for (p = server_info; p != nullptr; p = p->ai_next) {
        if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
            perror("server: socket");
            printf("server socket error\n");
            continue;
        }
        
        if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) {
            close(sockfd);
            perror("setsockopt");
            printf("set socket options error\n");
            exit(1);
        }
        
        if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
            close(sockfd);
            perror("server: bind");
            printf("server binding error\n");
            continue;
        }
        
        break;
    }
    
    freeaddrinfo(server_info);
    
    if (p == nullptr) {
        fprintf(stderr, "server: failed to bind\n");
        exit(1);
    }
    
    if (listen(sockfd, BACKLOG) == -1) {
        perror("listen");
        exit(1);
    }
    
    sa.sa_handler = Server::sigchld_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    if (sigaction(SIGCHLD, &sa, nullptr) == -1) {
        perror("sigaction");
        exit(1);
    }
    
    pthread_t listeningThread;
    int *new_sock = (int *)malloc(1);
    *new_sock = sockfd;
    if (pthread_create(&listeningThread, NULL, Server::listening_handler, (void *)new_sock) >= 0) {
        
    }
}

void *Server::listening_handler(void *sockfd) {
    int newfd;
    struct sockaddr_storage their_addr;
    socklen_t sin_size;
    int mysock = *(int*)sockfd;
    socketList *lastSocket = clients;
    
    sin_size = sizeof(their_addr);
    
    while (true) {
        newfd = accept(mysock, (struct sockaddr *)&their_addr, &sin_size);
        
        lastSocket->next = (socketList *)malloc(sizeof(socketList));
        lastSocket = lastSocket->next;
        lastSocket->sockfd = newfd;
        lastSocket->next = NULL;
        
        if (newConnectionCallback != NULL) {
            newConnectionCallback();
        }
        
        int *newnewfd = (int *) malloc(1);
        *newnewfd = newfd;
        pthread_t snifferThread;
        if (pthread_create(&snifferThread, NULL, Server::connection_handler, (void *)newnewfd) < 0) {
            
        }
    }
    
    return 0;
}

void *Server::connection_handler(void *socket_desc)
{
    //Get the socket descriptor
    int sock = *(int*)socket_desc;
    ssize_t read_size;
    char client_message[2000];
    smsg *lastMsg = messages;
    
    while( (read_size = recv(sock , client_message , 2000 , 0)) > 0 )
    {
        //Send the message back to client
        lastMsg->next = (smsg *)malloc(sizeof(smsg));
        char mes_cpy[500] = {'\0', };
        strcpy(mes_cpy, client_message);
        lastMsg->next->message = (char *)&mes_cpy;
        lastMsg->next->next = NULL;
        lastMsg = lastMsg->next;
        if (newMessageCallback != NULL) {
            newMessageCallback(lastMsg);
        }
        bzero(client_message, 2000);
    }
    
    if(read_size == 0)
    {
        puts("Client disconnected");
        fflush(stdout);
    }
    else if(read_size == -1)
    {
        perror("recv failed");
    }
    
    //free(socket_desc);
    
    return 0;
}

void Server::writeToAll(char *message)
{
    socketList *client = clients;
    if (clients != NULL) {
        while ((client = client->next) != NULL) {
            write(client->sockfd, message, strlen(message));
        }
    }
}

void Server::sigchld_handler(int s)
{
    int saved_errno = errno;
    while(waitpid(-1, NULL, WNOHANG) > 0);
    
    errno = saved_errno;
}
