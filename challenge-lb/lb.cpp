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
#include <fcntl.h>

class LoadBalancer {
private:
    int server_fd;
    int port;
    std::string backend_host;
    int backend_port;

public:
    LoadBalancer(int port = 80, std::string backend_host = "127.0.0.1", int backend_port = 8080) 
        : port(port), backend_host(backend_host), backend_port(backend_port), server_fd(-1) {}

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

        std::cout << "Load balancer listening on port " << port << std::endl;
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
            std::thread client_thread(&LoadBalancer::handleClient, this, client_fd, client_addr);
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

        // Forward request to backend server
        std::string response = forwardToBackend(request);

        if (!response.empty()) {
            // Extract the actual message content from backend response
            std::string backend_message = extractBackendMessage(response);
            std::cout << "[LB] : [" << backend_message << "|BE] " << std::endl;

            // Modify response to include load balancer marker
            std::string modified_response = addLoadBalancerMarker(response);
            
            // Send modified response back to client
            send(client_fd, modified_response.c_str(), modified_response.length(), 0);
        } else {
            // Send error response
            std::string error_response = "HTTP/1.1 502 Bad Gateway\r\n\r\n[LB] : [Backend server unavailable|BE] Backend server unavailable";
            std::cout << "[LB] : [Backend server unavailable|BE] Backend server unavailable" << std::endl;
            send(client_fd, error_response.c_str(), error_response.length(), 0);
        }

        close(client_fd);
    }

    std::string extractBackendMessage(const std::string& response) {
        // Find the end of HTTP headers (double CRLF)
        size_t header_end = response.find("\r\n\r\n");
        if (header_end == std::string::npos) {
            // If no proper HTTP headers found, return the whole response
            return response;
        }
        
        // Extract just the body content (the actual message from backend)
        std::string body = response.substr(header_end + 4);
        return body;
    }

    std::string addLoadBalancerMarker(const std::string& response) {
        // Find the end of HTTP headers (double CRLF)
        size_t header_end = response.find("\r\n\r\n");
        if (header_end == std::string::npos) {
            // If no proper HTTP headers found, just wrap the message
            return "[LB] : [" + response + "|BE] " + response;
        }
        
        // Split headers and body
        std::string headers = response.substr(0, header_end + 4);
        std::string body = response.substr(header_end + 4);
        
        // Wrap the backend message with load balancer marker using actual message
        std::string marked_body = "[LB] : [" + body + "|BE] ";
        
        // Update Content-Length header to match new body length
        std::string updated_headers = updateContentLength(headers, marked_body.length());
        
        return updated_headers + marked_body;
    }

    std::string updateContentLength(const std::string& headers, size_t new_length) {
        std::string updated_headers = headers;
        
        // Find Content-Length header
        size_t content_length_pos = updated_headers.find("Content-Length:");
        if (content_length_pos != std::string::npos) {
            // Find the end of the Content-Length line
            size_t line_end = updated_headers.find("\r\n", content_length_pos);
            if (line_end != std::string::npos) {
                // Replace the Content-Length value
                std::string new_content_length = "Content-Length: " + std::to_string(new_length);
                updated_headers.replace(content_length_pos, line_end - content_length_pos, new_content_length);
            }
        }
        
        return updated_headers;
    }

    std::string forwardToBackend(const std::string& request) {
        int backend_fd = socket(AF_INET, SOCK_STREAM, 0);
        if (backend_fd == -1) {
            return "";
        }

        struct sockaddr_in backend_addr;
        backend_addr.sin_family = AF_INET;
        backend_addr.sin_port = htons(backend_port);
        if (inet_pton(AF_INET, backend_host.c_str(), &backend_addr.sin_addr) <= 0) {
            close(backend_fd);
            return "";
        }

        if (connect(backend_fd, (struct sockaddr*)&backend_addr, sizeof(backend_addr)) < 0) {
            close(backend_fd);
            return "";
        }

        // Send request to backend
        send(backend_fd, request.c_str(), request.length(), 0);

        // Read response from backend
        char buffer[4096] = {0};
        std::string response;
        ssize_t bytes_read;
        
        while ((bytes_read = recv(backend_fd, buffer, sizeof(buffer) - 1, 0)) > 0) {
            response.append(buffer, bytes_read);
            memset(buffer, 0, sizeof(buffer));
        }

        close(backend_fd);
        return response;
    }

    ~LoadBalancer() {
        if (server_fd != -1) {
            close(server_fd);
        }
    }
};

int main(int argc, char* argv[]) {
    int port = 80;
    std::string backend_host = "127.0.0.1";
    int backend_port = 8080;

    // Parse command line arguments if provided
    if (argc >= 2) {
        port = std::stoi(argv[1]);
    }
    if (argc >= 4) {
        backend_host = argv[2];
        backend_port = std::stoi(argv[3]);
    }

    LoadBalancer lb(port, backend_host, backend_port);
    
    if (!lb.start()) {
        return 1;
    }

    lb.run();
    return 0;
}
