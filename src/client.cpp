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
    clientclass client = clientclass(std::stoi(std::string(argv[1])), std::string(argv[2]));
    
    // Connect to server
    try {
        client.connectToServer();
    } catch (std::runtime_error ret) {
        std::cerr << ret.what() << " with errno -1\n";
        return -1;
    }

    std::string userInput;

    do {
        std::cout << "> ";
        std::getline(std::cin, userInput);
        // Send string to server
        try {
            client.sendToServer(userInput);
        } catch (std::runtime_error ret) {
            std::cerr << ret.what() << " with errno -1\n";
        }
        // Recieve response
        try {
            client.receive();
        } catch (std::runtime_error ret) {
            std::cerr << ret.what() << " with errno -1\n";
            break;
        }
        std::cout << "Server: " << std::string(client.getBuf(), client.getBytes()) << "\r\n";
    } while (true);

    // Close the socket when we're done
    close(client.getSock());

    return 0;
}