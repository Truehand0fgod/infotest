#include <iostream>
#include <vector>
#include <cstring>
#include <system_error>
#include <sstream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "mylib.h"


void process_data(const std::string data) {
    if (my_func3(data)) {
        std::cout << "Data received successfuly" << std::endl;
    }
    else {
        std::cerr << "Error: incorrect data" << std::endl;
    }
}

void receive_from_program1() {
    try {
        const int server_port = 12345;
        int socket_fd = socket(AF_INET, SOCK_STREAM, 0);
        if (socket_fd == -1) {
            throw std::system_error(errno, std::system_category(), "Error creating socket");
        }

        struct sockaddr_in server_addr;
        std::memset(&server_addr, 0, sizeof(server_addr));
        server_addr.sin_family = AF_INET;
        server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
        server_addr.sin_port = htons(server_port);

        if (bind(socket_fd, reinterpret_cast<struct sockaddr*>(&server_addr), sizeof(server_addr)) == -1) {
            throw std::system_error(errno, std::system_category(), "Error binding socket");
        }

        if (listen(socket_fd, 1) == -1) {
            throw std::system_error(errno, std::system_category(), "Error listening on socket");
        }

        while (true) {
            int client_fd = accept(socket_fd, nullptr, nullptr);
            if (client_fd == -1) {
                throw std::system_error(errno, std::system_category(), "Error accepting connection");
            }

            std::vector<char> buffer(1024);
            int received = recv(client_fd, buffer.data(), buffer.size(), 0);
            if (received == -1) {
                std::cerr << "Connection error: " << std::strerror(errno) << std::endl;
                close(client_fd);
                continue;
            }

            std::string data(buffer.begin(), buffer.begin() + received);
            process_data(data);
            close(client_fd);
        }
    }
    catch (const std::system_error& e) {
        std::cerr << "Error occur while receiving data: " << e.what() << std::endl;
    }
    catch (const std::exception& e) {
        std::cerr << "Error occur while receiving data: " << e.what() << std::endl;
    }
}

int main() {
    receive_from_program1();
    return 0;
}
