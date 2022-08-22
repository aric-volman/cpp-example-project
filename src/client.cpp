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

#include "../lib/src/clientclass.hpp"

int main (int argc, char *argv[]) {
    int port;
    std::string ipaddr;
    
    try {
        if (argc != 3) {
            throw std::runtime_error("Client not given enough arguments");
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

    clientclass client = clientclass(port, ipaddr);
    
    // Connect to server
    try {
        client.connectToServer();
    } catch (std::runtime_error ret) {
        std::cerr << ret.what() << " with errno -1\n";
        return -1;
    }

    std::string userInput;

    while (true) {
        std::string received;
        std::cout << "> ";
        std::getline(std::cin, userInput);
        
        // Send string to server & receive response
        try {
            client.sendToServer(userInput);
            received = client.receive();
        } catch (std::runtime_error ret) {
            std::cerr << ret.what() << " with errno -1\n";
            break;
        }

        std::cout << "Server: " << received << "\r\n";
    }

    // Close the socket when we're done
    client.closeConnection();

    return 0;
}