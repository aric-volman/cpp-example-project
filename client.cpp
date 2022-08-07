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

#include "client.hpp"

int main() {
    // Create a socket
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1) {
        return -1;
    }

    // Create hint
    sockaddr_in hint;
    hint.sin_family = AF_INET;
    hint.sin_port = htons(PORT);
    inet_pton(AF_INET, (char*)IPADDR, &hint.sin_addr);

    // Connect to the server
    int connectRes = connect(sock, (struct sockaddr*)&hint, sizeof(hint));
    if (connectRes == -1) {
        return 1;
    }

    char buf[4096];
    std::string userInput;

    do {
        std::cout << "> ";
        std::getline(std::cin, userInput);
        // Send string to server
        int sendRes = send(sock, userInput.c_str(), userInput.size() + 1, 0);
        if (sendRes == -1) {
            std::cout << "Could not send!\r\n";
            continue;
        }
        // Recieve response
        memset(buf, 0, 4096);
        int bytes = recv(sock, buf, 4096, 0);
        std::cout << "Server: " << std::string(buf, bytes) << "\r\n";
    } while (true);

    close(sock);

    return 0;
}