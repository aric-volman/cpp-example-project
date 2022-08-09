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
        int e = client.connectToServer();
        if (e < 0) {throw e;}
    } catch (int ret) {
        return ret;
    }
    // int sock = client.getSock();

    char buf[4096];
    std::string userInput;

    do {
        std::cout << "> ";
        std::getline(std::cin, userInput);
        // Send string to server
        int sendRes = send(client.getSock(), userInput.c_str(), userInput.size() + 1, 0);
        if (sendRes == -1) {
            std::cerr << "Sending failed with errno " << -1 << "\n";
            continue;
        }
        // Recieve response
        memset(buf, 0, 4096);
        int bytes = recv(client.getSock(), buf, 4096, 0);
        std::cout << "Server: " << std::string(buf, bytes) << "\r\n";
    } while (true);

    // Close the socket when we're done
    close(client.getSock());

    return 0;
}