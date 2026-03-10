# Multithreaded Chat Application

A real-time TCP-based chat application in C++ supporting multiple simultaneous clients with message broadcasting.

![Language](https://img.shields.io/badge/Language-C++-blue)
![Platform](https://img.shields.io/badge/Platform-Windows-lightgrey)
![Networking](https://img.shields.io/badge/Networking-Winsock2-green)

## Features

- **Multi-client support** — Server handles many clients concurrently using Win32 threads (`CreateThread`)
- **Message broadcasting** — Messages from any client are relayed to all other connected clients
- **Thread-safe** — Client list protected with `CRITICAL_SECTION` to prevent race conditions
- **Graceful disconnect** —  Server detects disconnections and notifies remaining clients
- **Username support** — Each client identifies with a username on connection


## Directory Structure

```
MultithreadedChat/
├── server/
│   ├── main.cpp            # Server entry point
│   ├── ChatServer.h        # Server class declaration
│   └── ChatServer.cpp      # Server class implementation
├── client/
│   ├── main.cpp            # Client entry point
│   ├── ChatClient.h        # Client class declaration
│   └── ChatClient.cpp      # Client class implementation
├── Makefile                # Build automation (Linux/macOS)
└── README.md               # This file
```

## Building

### Windows (PowerShell with g++/MinGW)

```powershell
# Create output directory
New-Item -ItemType Directory -Force -Path bin

# Build server
g++ -std=c++11 -Wall -o bin/server.exe server/main.cpp server/ChatServer.cpp -lws2_32

# Build client
g++ -std=c++11 -Wall -o bin/client.exe client/main.cpp client/ChatClient.cpp -lws2_32
```

### Linux / macOS

```bash
make
```

## Running

### 1. Start the Server

```bash
# Default port 54000
./bin/server

# Or specify a port
./bin/server 8080
```

### 2. Connect Clients

Open a new terminal for each client:

```bash
# Connect to localhost on default port
./bin/client

# Or specify server IP and port
./bin/client 127.0.0.1 8080
```

You will be prompted for a username. Type messages and press Enter to send. Type `/quit` to disconnect.

## Architecture

### Server
1. Creates a TCP socket, binds to the specified port, and listens for connections.
2. On each new client connection, spawns a detached `CreateThread` to handle that client.
3. The client handler reads the username, then enters a recv loop.
4. Received messages are broadcast to **all other** connected clients.
5. On disconnect, the client socket is removed from the shared list and closed.

### Client
1. Connects to the server and sends the username as the first message.
2. Spawns a **receive thread** that listens for incoming messages and prints them.
3. The **main thread** reads user input and sends it to the server.
4. Typing `/quit` or pressing Ctrl+C disconnects gracefully.

## Protocol

| Step | Direction | Data |
|------|-----------|------|
| 1 | Client → Server | Username (plain text, max 32 chars) |
| 2 | Client → Server | Chat messages (newline-terminated) |
| 3 | Server → Clients | `[username] message` or `[username joined/left the chat]` |
