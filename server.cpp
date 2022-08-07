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

#include "server.hpp"

int main () {
    // Initialize custom message
    std::string messageToSendBack = "recieved.";
    // Create listening socket
    int listeningSock = socket(AF_INET, SOCK_STREAM, 0);
    
    if (listeningSock == -1) {
        std::cerr << "Can't create listening socket\n";
        return -1;
    }

    // Bind socket to address and port
    struct sockaddr_in hint;
    hint.sin_family = AF_INET;
    hint.sin_port = htons(PORT);
    inet_pton(AF_INET, IPADDR, &hint.sin_addr);

    if (bind(listeningSock, (struct sockaddr*)&hint, sizeof(hint)) == -1) {
        std::cerr << "Can't bind to port\n";
        return -2;
    }

    // Listen to the socket
    if (listen(listeningSock, SOMAXCONN) == -1) {
        std::cerr << "Can't listen to socket\n";
        return -3;
    }

    sockaddr_in client;
    socklen_t clientSize;
    char host[NI_MAXHOST];
    char svc[NI_MAXSERV];

    // Accept the call
    int clientSock = accept(listeningSock, (struct sockaddr*)&client, &clientSize);
    
    if (clientSock == -1) {
        std::cerr << "Problem with client connection\r\n";
        return -4;
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

    // Display message
    char buf[4096];
    while(true) {
        // Clear the buffer
        memset(buf, 0, 4096);
        // Wait for the message & display
        int bytes = recv(clientSock, buf, 4096, 0);
        if ((bytes == -1) || (bytes == 0)) {
            std::cerr << "There was a connection issue, or the client disconnected" << "\r\n";
            while (true) {
                // Close current socket
                close(clientSock);
                // Create listening socket
                int listeningSock = socket(AF_INET, SOCK_STREAM, 0);
                
                if (listeningSock == -1) {
                    std::cerr << "Can't create listening socket\n";
                    return -1;
                }

                // Bind socket to address and port
                struct sockaddr_in hint;
                hint.sin_family = AF_INET;
                hint.sin_port = htons(PORT);
                inet_pton(AF_INET, IPADDR, &hint.sin_addr);

                if (bind(listeningSock, (struct sockaddr*)&hint, sizeof(hint)) == -1) {
                    std::cerr << "Can't bind to port\n";
                    return -2;
                }

                // Listen to the socket
                if (listen(listeningSock, SOMAXCONN) == -1) {
                    std::cerr << "Can't listen to socket\n";
                    return -3;
                }
                sockaddr_in client;
                socklen_t clientSize;
                char host[NI_MAXHOST];
                char svc[NI_MAXSERV];

                // Accept the call
                clientSock = accept(listeningSock, (struct sockaddr*)&client, &clientSize);
                if (clientSock == -1) {
                    std::cerr << "Problem with client connection\r\n";
                    return -4;
                } else {
                    break;
                }
            }
        }
        std::string recieved = std::string(buf, 0, bytes);
        
        if ((recieved == "Quit") || (recieved == "quit")) {
            std::cout << "Quit recieved, closing...\r\n";
            break;
        }
        std::cout << "Received from server: " << recieved << "\r\n";
        // Send back the message
        send(clientSock, messageToSendBack.c_str(), messageToSendBack.size() + 1, 0);
    }
    close(clientSock);
    return 0;
}