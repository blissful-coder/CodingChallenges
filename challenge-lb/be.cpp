#include <iostream>
#include <string>
#include <thread>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

class BackendServer {
private:
    int server_fd;
    int port;

public:
    BackendServer(int port = 8080) : port(port), server_fd(-1) {}

    bool start() {
        // Create socket
        server_fd = socket(AF_INET, SOCK_STREAM, 0);
        if (server_fd == -1) {
            std::cerr << "Failed to create socket" << std::endl;
            return false;
        }

        // Set socket options
        int opt = 1;
        if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) {
            std::cerr << "Failed to set socket options" << std::endl;
            return false;
        }

        // Bind socket
        struct sockaddr_in address;
        address.sin_family = AF_INET;
        address.sin_addr.s_addr = INADDR_ANY;
        address.sin_port = htons(port);

        if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
            std::cerr << "Failed to bind socket to port " << port << std::endl;
            return false;
        }

        // Listen for connections
        if (listen(server_fd, 10) < 0) {
            std::cerr << "Failed to listen on socket" << std::endl;
            return false;
        }

        std::cout << "Backend server listening on port " << port << std::endl;
        return true;
    }

    void run() {
        while (true) {
            struct sockaddr_in client_addr;
            socklen_t client_len = sizeof(client_addr);
            
            int client_fd = accept(server_fd, (struct sockaddr*)&client_addr, &client_len);
            if (client_fd < 0) {
                std::cerr << "Failed to accept client connection" << std::endl;
                continue;
            }

            // Handle client in separate thread
            std::thread client_thread(&BackendServer::handleClient, this, client_fd, client_addr);
            client_thread.detach();
        }
    }

    void handleClient(int client_fd, struct sockaddr_in client_addr) {
        char buffer[4096] = {0};
        ssize_t bytes_read = recv(client_fd, buffer, sizeof(buffer) - 1, 0);
        
        if (bytes_read <= 0) {
            close(client_fd);
            return;
        }

        std::string request(buffer);
        std::string client_ip = inet_ntoa(client_addr.sin_addr);

        // Log the incoming request
        std::cout << "Received request from " << client_ip << std::endl;
        std::cout << request << std::endl;

        // Create HTTP response
        std::string response = "HTTP/1.1 200 OK\r\n";
        response += "Content-Type: text/plain\r\n";
        response += "Content-Length: 25\r\n";
        response += "Connection: close\r\n";
        response += "\r\n";
        response += "Hello From Backend Server";

        // Send response
        send(client_fd, response.c_str(), response.length(), 0);
        
        std::cout << "Replied with a hello message" << std::endl << std::endl;
        close(client_fd);
    }

    ~BackendServer() {
        if (server_fd != -1) {
            close(server_fd);
        }
    }
};

int main(int argc, char* argv[]) {
    int port = 8080;

    // Parse command line arguments if provided
    if (argc >= 2) {
        port = std::stoi(argv[1]);
    }

    BackendServer server(port);
    
    if (!server.start()) {
        return 1;
    }

    server.run();
    return 0;
}
