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

    int port;
    std::string ipaddr;
    
    try {
        if (argc != 3) {
            throw std::runtime_error("Server not given enough arguments");
        }
        port = std::stoi(argv[1]);
        ipaddr = std::string(argv[2]);
    } catch (std::invalid_argument ret) {
        std::cerr << ret.what() << " returned with errno -1\n";
        return -1;
    } catch (std::runtime_error ret) {
        std::cerr << ret.what() << " with errno -1\n";
        return -1;
    }

    serverclass server = serverclass(port, ipaddr);

    // Display message

    while(true) {
        std::string received;
        // Wait for the message & display
        try {
            server.listenAndConnect();
        } catch (std::runtime_error ret) {
            std::cerr << ret.what() << " with errno -1\n";
            break; // Exits loop and program
        }

        try {
            received = server.receive();
        } catch (std::runtime_error ret) {
            // Does not exit and continues
            std::cerr << ret.what() << " with errno -1\n";
        }


        if ((received == "Quit") || (received == "quit")) {
            std::cout << "Quit recieved, closing...\r\n";
            break;
        }

        if (received != "") {
            std::cout << "Received from server: " << received << "\r\n";
        }
        
        // Send back the message
        server.sendToClient("received.");
    }

    // Close the socket when we're done
    server.closeConnection();
    return 0;
}