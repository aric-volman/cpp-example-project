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

#include "../lib/src/serverclass.hpp"

int main (int argc, char *argv[]) {
    serverclass server = serverclass(std::stoi(std::string(argv[1])), std::string(argv[2]));

    // Initialize custom message
    std::string messageToSendBack = "recieved.";
    try {
        int e = server.listenAndConnect();
        if (e < 0) {throw e;};
    } catch (int ret) {
        return ret;
    }

    // int sock = server.getClientSock();

    int bytes;

    // Display message
    char buf[4096];
    while(true) {
        // Clear the buffer
        memset(buf, 0, 4096);

        try {
            // Wait for the message & display
            bytes = recv(server.getClientSock(), buf, 4096, 0);
            if (bytes == -1) {
                std::cerr << "Connection failed with errno " << -1 << "\n";
                throw bytes;
            } else if (bytes == 0) {
                std::cerr << "Client disconnected with errno " << 0 << "\n";
                throw bytes;
            }
        } catch (int ret) {
            while (true) {
                // Close current socket
                close(server.getClientSock());
                // Connect back
                try {
                    int e = server.listenAndConnect();
                    if (e < 0) {throw e;};
                    if (e == 0) {break;};
                } catch (int ret) {
                    return ret;
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
        send(server.getClientSock(), messageToSendBack.c_str(), messageToSendBack.size() + 1, 0);
    }

    // Close the socket when we're done
    close(server.getClientSock());
    return 0;
}