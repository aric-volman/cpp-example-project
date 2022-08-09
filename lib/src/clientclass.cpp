#include <iostream>

#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>

#include "clientclass.hpp"

clientclass::clientclass(int port, std::string ipaddr) {
    this->port = port;
    this->ipaddr = ipaddr;
};

int clientclass::getSock() {
    return sock;
}

int clientclass::connectToServer() {
    // Create a socket
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1) {
        std::cerr << "Socket creation failed with errno " << -1 << "\n";
        return -1;
    }

    // Create hint
    sockaddr_in hint;
    hint.sin_family = AF_INET;
    hint.sin_port = htons(port);
    inet_pton(AF_INET, (char*)ipaddr.c_str(), &hint.sin_addr);

    // Connect to the server
    int connectRes = connect(sock, (struct sockaddr*)&hint, sizeof(hint));
    if (connectRes == -1) {
        std::cerr << "Connection failed with errno " << -1 << "\n";
        return -1;
    }

    return 0;
}