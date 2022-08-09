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

#include "serverclass.hpp"

int main (int argc, char *argv[]) {
    serverclass* server = new serverclass(std::stoi(std::string(argv[1])), std::string(argv[2]));
    // Initialize custom message
    std::string messageToSendBack = "recieved.";
    try {
        int e = server->listenAndConnect();
        if (e < 0) {throw e;};
    } catch (int ret) {
        return ret;
    }
    // Display message
    char buf[4096];
    while(true) {
        // Clear the buffer
        memset(buf, 0, 4096);
        // Wait for the message & display
        int bytes = recv(server->getClientSock(), buf, 4096, 0);
        if ((bytes == -1) || (bytes == 0)) {
            std::cerr << "There was a connection issue, or the client disconnected" << "\r\n";
            while (true) {
                // Close current socket
                close(server->getClientSock());
                // Connect back
                try {
                    int e = server->listenAndConnect();
                    if (e < 0) {throw e;};
                } catch (int ret) {
                    return ret;
                }
                break;
            }
        }
        std::string recieved = std::string(buf, 0, bytes);
        
        if ((recieved == "Quit") || (recieved == "quit")) {
            std::cout << "Quit recieved, closing...\r\n";
            break;
        }
        std::cout << "Received from server: " << recieved << "\r\n";
        // Send back the message
        send(server->getClientSock(), messageToSendBack.c_str(), messageToSendBack.size() + 1, 0);
    }
    close(server->getClientSock());
    return 0;
}