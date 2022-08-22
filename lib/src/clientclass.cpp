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

void clientclass::closeConnection() {
    close(sock);
}

std::string clientclass::receive() {
    // Clear the buffer
    memset(buf, 0, 4096);
    bytes = recv(sock, buf, 4096, 0);
    if (bytes == -1) {
        throw std::runtime_error("Connection failed");
    }
    if (bytes == 0) {
        throw std::runtime_error("Server disconnected");
    }
    return std::string(buf, bytes);
}

void clientclass::sendToServer(std::string input) {
    // Send string to server
    int sendRes = send(sock, input.c_str(), input.size() + 1, 0);
    if (sendRes == -1) {
        throw std::runtime_error("Sending failed");
    }
}

void clientclass::connectToServer() {
    // Create a socket
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1) {
        throw std::runtime_error("Socket creation failed");
    }

    // Create hint
    sockaddr_in hint;
    hint.sin_family = AF_INET;
    hint.sin_port = htons(port);
    inet_pton(AF_INET, (char*)ipaddr.c_str(), &hint.sin_addr);

    // Connect to the server
    int connectRes = connect(sock, (struct sockaddr*)&hint, sizeof(hint));
    if (connectRes == -1) {
        throw std::runtime_error("Connection failed");
    }
}