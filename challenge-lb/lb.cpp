#include <iostream>
#include <string>
#include <thread>
#include <vector>
#include <sstream>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netdb.h>

class LoadBalancer {
private:
    int listen_port;
    std::string backend_host;
    int backend_port;
    int server_socket;

public:
    LoadBalancer(int port, const std::string& host, int b_port) 
        : listen_port(port), backend_host(host), backend_port(b_port), server_socket(-1) {}

    ~LoadBalancer() {
        if (server_socket != -1) {
            close(server_socket);
        }
    }

    bool start() {
        // Create socket
        server_socket = socket(AF_INET, SOCK_STREAM, 0);
        if (server_socket == -1) {
            std::cerr << "Failed to create socket" << std::endl;
            return false;
        }

        // Set socket options to reuse address
        int opt = 1;
        if (setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
            std::cerr << "Failed to set socket options" << std::endl;
            return false;
        }

        // Bind socket
        struct sockaddr_in server_addr;
        server_addr.sin_family = AF_INET;
        server_addr.sin_addr.s_addr = INADDR_ANY;
        server_addr.sin_port = htons(listen_port);

        if (bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
            std::cerr << "Failed to bind socket to port " << listen_port << std::endl;
            return false;
        }

        // Listen for connections
        if (listen(server_socket, 10) < 0) {
            std::cerr << "Failed to listen on socket" << std::endl;
            return false;
        }

        std::cout << "Load balancer listening on port " << listen_port << std::endl;
        return true;
    }

    void run() {
        while (true) {
            struct sockaddr_in client_addr;
            socklen_t client_len = sizeof(client_addr);
            
            int client_socket = accept(server_socket, (struct sockaddr*)&client_addr, &client_len);
            if (client_socket < 0) {
                std::cerr << "Failed to accept connection" << std::endl;
                continue;
            }

            // Handle client in a separate thread
            std::thread client_thread(&LoadBalancer::handle_client, this, client_socket, client_addr);
            client_thread.detach();
        }
    }

private:
    void handle_client(int client_socket, struct sockaddr_in client_addr) {
        char client_ip[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &client_addr.sin_addr, client_ip, INET_ADDRSTRLEN);

        // Read request from client
        char buffer[4096];
        ssize_t bytes_received = recv(client_socket, buffer, sizeof(buffer) - 1, 0);
        if (bytes_received <= 0) {
            close(client_socket);
            return;
        }
        buffer[bytes_received] = '\0';

        // Log the incoming request
        std::cout << "Received request from " << client_ip << std::endl;
        std::cout << buffer << std::endl;

        // Forward request to backend server
        std::string response = forward_to_backend(buffer);
        
        if (!response.empty()) {
            // Log response from backend
            std::string status_line = response.substr(0, response.find('\n'));
            std::cout << "Response from server: " << status_line << std::endl;
            
            // Send response back to client
            send(client_socket, response.c_str(), response.length(), 0);
        } else {
            // Send error response if backend is unavailable
            std::string error_response = "HTTP/1.1 502 Bad Gateway\r\n\r\nBackend server unavailable";
            send(client_socket, error_response.c_str(), error_response.length(), 0);
        }

        close(client_socket);
    }

    std::string forward_to_backend(const std::string& request) {
        // Create socket to backend
        int backend_socket = socket(AF_INET, SOCK_STREAM, 0);
        if (backend_socket == -1) {
            std::cerr << "Failed to create backend socket" << std::endl;
            return "";
        }

        // Resolve backend host
        struct hostent* host_entry = gethostbyname(backend_host.c_str());
        if (host_entry == nullptr) {
            std::cerr << "Failed to resolve backend host: " << backend_host << std::endl;
            close(backend_socket);
            return "";
        }

        // Connect to backend
        struct sockaddr_in backend_addr;
        backend_addr.sin_family = AF_INET;
        backend_addr.sin_port = htons(backend_port);
        memcpy(&backend_addr.sin_addr, host_entry->h_addr_list[0], host_entry->h_length);

        if (connect(backend_socket, (struct sockaddr*)&backend_addr, sizeof(backend_addr)) < 0) {
            std::cerr << "Failed to connect to backend server" << std::endl;
            close(backend_socket);
            return "";
        }

        // Send request to backend
        if (send(backend_socket, request.c_str(), request.length(), 0) < 0) {
            std::cerr << "Failed to send request to backend" << std::endl;
            close(backend_socket);
            return "";
        }

        // Read response from backend
        std::string response;
        char buffer[4096];
        ssize_t bytes_received;
        
        // Read the complete response
        while ((bytes_received = recv(backend_socket, buffer, sizeof(buffer), 0)) > 0) {
            response.append(buffer, bytes_received);
        }

        close(backend_socket);
        return response;
    }
};

int main(int argc, char* argv[]) {
    int listen_port = 80;
    std::string backend_host = "127.0.0.1";
    int backend_port = 8080;

    // Parse command line arguments
    if (argc >= 2) {
        listen_port = std::stoi(argv[1]);
    }
    if (argc >= 3) {
        backend_host = argv[2];
    }
    if (argc >= 4) {
        backend_port = std::stoi(argv[3]);
    }

    LoadBalancer lb(listen_port, backend_host, backend_port);
    
    if (!lb.start()) {
        return 1;
    }

    lb.run();
    return 0;
}