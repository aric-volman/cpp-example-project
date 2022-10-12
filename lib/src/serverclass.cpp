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
#include <shared_mutex>
#include <thread>
#include <map>

#include "serverclass.hpp"
std::map<std::string,int> iptable;
std::shared_timed_mutex tablemutex;

serverclass::serverclass(int port, std::string ipaddr) {
    this->port = port;
    this->ipaddr = ipaddr;
}

void serverclass::closeConnection() {
    close(clientSock);
}

void serverclass::sendToClient(std::string message) {
    send(clientSock, message.c_str(), message.size() + 1, 0);
}

std::string serverclass::receive() {
    // Clear the buffer
    memset(buf, 0, 4096);
    bytes = recv(clientSock, buf, 4096, 0);
    if (bytes == -1) {
        throw std::runtime_error("Connection failed");
    }
    if (bytes == 0) {
        throw std::runtime_error("Client disconnected");
    }
    return std::string(buf, 0, bytes);
}
void serverclass::listenAndConnect() {
    if (bytes == 0 || bytes == -1) {
        close(clientSock);
        while (true) {
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
            
            /*
            if (clientSock == -1) {
                throw std::runtime_error("Client connection failed");
            }*/

            close(listeningSock);

            // Connection code

            memset(host, 0, NI_MAXHOST);
            memset(svc, 0, NI_MAXSERV);

            int result = getnameinfo((struct sockaddr*)&client, sizeof(client), host, NI_MAXHOST, svc, NI_MAXSERV, 0);

            if (result) {
                std::cout << host << " connected on " << svc << "\r\n";
                addToIPtable(svc, clientSock);
                break;
            } else {
                inet_ntop(AF_INET, &client.sin_addr, host, NI_MAXHOST);
                std::cout << host << " connected on " << ntohs(client.sin_port) << "\r\n";
                addToIPtable(std::to_string((client.sin_port)), clientSock);
                break;
            }
        }
    }
}

void serverclass::addToIPtable(std::string ip, int sock) {
    std::lock_guard<std::shared_timed_mutex> lock(tablemutex);
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    iptable[ip] = sock;
}


// Purpose of this method: To experiment with mutexes
void serverclass::listenToClient() {
    // Creates socket
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

    while (true) {

            // Listen to the socket
            if (listen(listeningSock, SOMAXCONN) == -1) {
                throw std::runtime_error("Socket listening failed");
            }
            std::cout << &hint.sin_addr << std::endl;
    }
}