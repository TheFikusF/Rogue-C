#include "./include/core/networking/Server.h"
#include <cstring>
#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>

using namespace Core::Networking;

void Server::Start(std::uint16_t port) {
 // creating socket
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);

    int flags = fcntl(serverSocket, F_GETFL, 0);
    fcntl(serverSocket, F_SETFL, flags | O_NONBLOCK);

    sockaddr_in serverAddress;
    memset(&serverAddress, 0, sizeof(serverAddress));
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(port);
    serverAddress.sin_addr.s_addr = INADDR_ANY;

    // binding socket.
    bind(serverSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress));

    // listening to the assigned socket
    listen(serverSocket, 5);

    
}

void Server::Step(float tick) {
    {
        sockaddr_in client_addr;
        socklen_t client_len = sizeof(client_addr);

        // Accept new connections
        int clientSocket = accept(serverSocket, (sockaddr*)&client_addr, &client_len);
        if (clientSocket >= 0) {
            std::cout << "Client connected\n";

            // Set the client socket to non-blocking mode
            int flags = fcntl(clientSocket, F_GETFL, 0);
            fcntl(clientSocket, F_SETFL, flags | O_NONBLOCK);
            clients[clientSocket] = 0;
        }
    }

    for (auto& [clientSocket, entity] : clients) {
        char buffer[1024];
        int bytes_received = recv(clientSocket, buffer, sizeof(buffer) - 1, 0);

        if (bytes_received > 0) {
            buffer[bytes_received] = '\0'; // Null-terminate the string
            std::cout << "Received: " << buffer << "\n";
        } else if (bytes_received == 0) {
            std::cout << "Client disconnected\n";
            close(clientSocket);
            clients.erase(clientSocket);
        } else if (errno != EWOULDBLOCK) {
            std::cerr << "Receive error\n";
        }

        send(clientSocket, "hello client, fuck you", 22, 0);
    }
}

void Server::Stop() {
    for (auto& [clientSocket, entity] : clients) {
       close(clientSocket);
    }

    close(serverSocket);
}
