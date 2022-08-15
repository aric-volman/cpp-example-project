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

#include "../lib/src/serverclass.hpp"

int main (int argc, char *argv[]) {

    if (argc != 3) {
        throw std::range_error("Not enough args");
    }

    int port = std::stoi(std::string(argv[1]));
    std::string ipaddr = std::string(argv[2]);

    serverclass server = serverclass(port, ipaddr);

    // Initialize custom message
    std::string messageToSendBack = "recieved.";
    try {
        server.listenAndConnect();
    } catch (std::runtime_error ret) {
        std::cerr << ret.what() << " with errno -1\n";
        return -1;
    }

    // Display message

    while(true) {
        // Wait for the message & display
        try {
            server.receive();
        } catch (std::runtime_error ret) {
            std::cerr << ret.what() << " with errno -1\n";
        }
        if (server.getBytes() == 0 || server.getBytes() == -1) {
            while (true) {
                // Close current socket
                close(server.getClientSock());
                // Connect back
                try {
                    int e = server.listenAndConnect();
                    if (e == 0) {break;};
                } catch (std::runtime_error ret) {
                    std::cerr << ret.what() << " with errno -1\n";
                    break;
                }
            }
        } else if (!(server.getBytes() == 0 || server.getBytes() == -1)) {
            std::string recieved = std::string(server.getBuf(), 0, server.getBytes());
            
            if ((recieved == "Quit") || (recieved == "quit")) {
                std::cout << "Quit recieved, closing...\r\n";
                break;
            }
            std::cout << "Received from server: " << recieved << "\r\n";
            // Send back the message
            send(server.getClientSock(), messageToSendBack.c_str(), messageToSendBack.size() + 1, 0);
        }
    }

    // Close the socket when we're done
    server.closeConnection();
    return 0;
}