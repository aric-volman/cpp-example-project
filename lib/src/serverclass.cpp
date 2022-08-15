#include <iostream>

#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdexcept>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>

#include "serverclass.hpp"

serverclass::serverclass(int port, std::string ipaddr) {
    this->port = port;
    this->ipaddr = ipaddr;
}

int serverclass::getListeningSock() {
    return listeningSock;
}

int serverclass::getClientSock() {
    return clientSock;
}

char* serverclass::getBuf() {
    return buf;
}

int serverclass::getBytes() {
    return bytes;
}

void serverclass::closeConnection() {
    close(clientSock);
}

int serverclass::receive() {
    // Clear the buffer
    memset(buf, 0, 4096);
    bytes = recv(getClientSock(), buf, 4096, 0);
    if (bytes == -1) {
        throw std::runtime_error("Connection failed");
    }
    if (bytes == 0) {
        throw std::runtime_error("Client disconnected");
    }
    return 0;
}
int serverclass::listenAndConnect() {
    // Create listening socket
    listeningSock = socket(AF_INET, SOCK_STREAM, 0);
    
    if (listeningSock == -1) {
        throw std::runtime_error("Listening socket creation failed");
    }

    // Bind socket to address and port
    struct sockaddr_in hint;
    hint.sin_family = AF_INET;
    hint.sin_port = htons(port);
    inet_pton(AF_INET, ipaddr.c_str(), &hint.sin_addr);

    if (bind(listeningSock, (struct sockaddr*)&hint, sizeof(hint)) == -1) {
        throw std::runtime_error("Port binding failed");
    }

    // Listen to the socket
    if (listen(listeningSock, SOMAXCONN) == -1) {
        throw std::runtime_error("Socket listening failed");
    }

    sockaddr_in client;
    socklen_t clientSize;
    char host[NI_MAXHOST];
    char svc[NI_MAXSERV];

    // Accept the call
    clientSock = accept(listeningSock, (struct sockaddr*)&client, &clientSize);
    
    if (clientSock == -1) {
        throw std::runtime_error("Client connection failed");
    }

    close(listeningSock);

    memset(host, 0, NI_MAXHOST);
    memset(svc, 0, NI_MAXSERV);

    int result = getnameinfo((struct sockaddr*)&client, sizeof(client), host, NI_MAXHOST, svc, NI_MAXSERV, 0);

    if (result) {
        std::cout << host << " connected on " << svc << "\r\n";
    } else {
        inet_ntop(AF_INET, &client.sin_addr, host, NI_MAXHOST);
        std::cout << host << " connected on " << ntohs(client.sin_port) << "\r\n";
    }

    return 0;
}