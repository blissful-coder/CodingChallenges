# Load Balancer Implementation

This project implements a basic load balancer that can receive incoming HTTP connections and forward them to a backend server.

## Features

- **Load Balancer (`lb`)**: Listens on a specified port and forwards requests to a backend server
- **Backend Server (`be`)**: Simple HTTP server that responds with "Hello From Backend Server"
- **Concurrency**: Both servers handle multiple clients using threading
- **Request Logging**: Detailed logging of incoming requests and responses

## Files

- `lb.cpp` - Load balancer implementation
- `be.cpp` - Backend server implementation
- `Makefile` - Build configuration
- `test.sh` - Automated test script
- `README.md` - This documentation

## Building

To compile both programs:

```bash
make
```

To clean build artifacts:

```bash
make clean
```

## Usage

### Backend Server

Start the backend server on port 8080:

```bash
./be 8080
```

The backend server will log incoming requests and respond with "Hello From Backend Server".

### Load Balancer

Start the load balancer on port 8000, forwarding to backend on 127.0.0.1:8080:

```bash
./lb 8000 127.0.0.1 8080
```

### Complete Test

1. Start the backend server:
```bash
./be 8080
```

2. In another terminal, start the load balancer:
```bash
./lb 8000 127.0.0.1 8080
```

3. In a third terminal, test the system:
```bash
curl http://localhost:8000/
```

## Expected Output

### Backend Server Console
```
Backend server listening on port 8080
Received request from 127.0.0.1
GET / HTTP/1.1
Host: localhost:8000
User-Agent: curl/8.7.1
Accept: */*

Replied with a hello message
```

### Load Balancer Console
```
Load balancer listening on port 8000
Received request from 127.0.0.1
GET / HTTP/1.1
Host: localhost:8000
User-Agent: curl/8.7.1
Accept: */*

Response from server: HTTP/1.1 200 OK
```

### Client Output
```
Hello From Backend Server
```

## Automated Testing

Run the automated test script:

```bash
./test.sh
```

This script will:
1. Clean up any existing processes
2. Start both servers
3. Make a test request
4. Display the response
5. Clean up processes

## Architecture

- The load balancer accepts incoming connections on the specified port
- Each client connection is handled in a separate thread for concurrency
- Requests are parsed and logged with client IP and full HTTP headers
- The load balancer opens a connection to the backend server
- The original request is forwarded to the backend
- The backend's response is forwarded back to the client
- Both servers use threading to handle multiple concurrent connections

## Command Line Arguments

### Load Balancer
- `./lb [listen_port] [backend_host] [backend_port]`
- Default: `./lb 80 127.0.0.1 8080`

### Backend Server
- `./be [port]`
- Default: `./be 8080`
