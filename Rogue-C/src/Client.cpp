#include "./include/core/networking/Client.h"
#include <cstring>
#include <string>
#include <format>
#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include "./include/core/networking/NetworkManager.h"

using namespace Core::Networking;

void Client::Start(std::uint16_t port) {
    clientSocket = socket(AF_INET, SOCK_STREAM, 0);

    int flags = fcntl(clientSocket, F_GETFL, 0);
    fcntl(clientSocket, F_SETFL, flags | O_NONBLOCK);

    sockaddr_in serverAddress;
    memset(&serverAddress, 0, sizeof(serverAddress));
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(8080);
    serverAddress.sin_addr.s_addr = INADDR_ANY;

    // sending connection request
    connect(clientSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress));
}

void Client::Step(float tick) {
    send(clientSocket, "str", 3, 0);

    char buffer[1024];
    int bytes_received = recv(clientSocket, buffer, sizeof(buffer) - 1, 0);

    if (bytes_received > 0) {
        buffer[bytes_received] = '\0'; // Null-terminate the string
        std::cout << "Received: " << buffer << "\n";
    } else if (bytes_received == 0) {
        std::cout << "Client disconnected\n";
        close(clientSocket);
        clientSocket = -1;
    } else if (errno != EWOULDBLOCK) {
        std::cerr << "Receive error\n";
        NetworkManager::Stop();
    }
}

void Client::Stop() {
    close(clientSocket);
}
