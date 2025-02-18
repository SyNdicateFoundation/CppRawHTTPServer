#include <cstring>
#include <iostream>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

int main() {
    const int sockserver = socket(AF_INET, SOCK_STREAM, 0);
    if (sockserver < 0) {
        perror("socket");
        exit(1);
    }

    sockaddr_in sock_addr{};
    sock_addr.sin_family = AF_INET;
    sock_addr.sin_port = htons(20008);
    sock_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    bind(sockserver, (struct sockaddr*)&sock_addr, sizeof(sock_addr));

    listen(sockserver, 5);
    while (true) {
        int client_socket = accept(sockserver, NULL, NULL);
        if (client_socket < 0) {
            perror("accept");
            continue;
        }
        char request[1024];
        memset(request, 0, sizeof(request));
        ssize_t b = recv(client_socket, request, sizeof(request), 0);
        std::string request_str = std::string(request);
        if (request_str.length() < 2) {
            close(client_socket);
            continue;
        }
        request[b] = '\0';
        std::cout << "Connection Handled! Request: " << request_str << std::endl;

        std::string response = "HTTP/1.0 200 OK\r\n Content-Type: text/html\r\n\r\n";

        send(client_socket, response.c_str(), response.length(), 0);
        send(client_socket, "test", std::string("test").length(), 0);
        close(client_socket);
    }

    return 0;
}