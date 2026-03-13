# 🚀 Multithreaded Chat Application (C++)

A **real-time TCP chat application built in C++** that supports multiple simultaneous clients using **multithreading and socket programming**.  
The server handles concurrent client connections and broadcasts messages between them in real time.

---

# ✨ Key Features

✅ **Multi-client support**  
Server handles multiple clients concurrently using **Win32 threads (`CreateThread`)**

✅ **Real-time message broadcasting**  
Messages sent by one client are instantly delivered to all other connected clients

✅ **Thread-safe architecture**  
Shared client list protected using **`CRITICAL_SECTION`**

✅ **Graceful client disconnects**  
Server detects client disconnections and informs remaining users

✅ **Username-based chat system**  
Each user joins the chat with a unique username

---

# 🏗 System Architecture

Client 1 ─┐
Client 2 ─┼──────► Chat Server ─────► Broadcast to all clients
Client 3 ─┘


**Workflow**

1. Server listens for incoming TCP connections
2. Each new client connection spawns a **separate thread**
3. Client sends username after connecting
4. Server receives messages and **broadcasts to all other clients**
5. If a client disconnects, the server removes it safely

---

## 📂 Project Structure

```bash
MultithreadedChat/
│
├── server/
│   ├── main.cpp            # Server entry point
│   ├── ChatServer.h        # Server class declaration
│   └── ChatServer.cpp      # Server class implementation
│
├── client/
│   ├── main.cpp            # Client entry point
│   ├── ChatClient.h        # Client class declaration
│   └── ChatClient.cpp      # Client class implementation
│
├── Makefile                # Build automation (Linux/macOS)
└── README.md               # Project documentation


---

# ⚙️ Technologies Used

| Technology | Purpose |
|------------|--------|
| C++ | Core programming language |
| TCP Sockets | Network communication |
| Win32 Threads | Concurrent client handling |
| CRITICAL_SECTION | Thread synchronization |

---

# 🛠 Build Instructions

## Windows (MinGW / g++)

```bash
# Create output directory
mkdir bin

# Compile server
g++ -std=c++11 -Wall -o bin/server.exe server/main.cpp server/ChatServer.cpp -lws2_32

# Compile client
g++ -std=c++11 -Wall -o bin/client.exe client/main.cpp client/ChatClient.cpp -lws2_32
Linux / macOS
make
▶ Running the Application
Start Server
./bin/server

or

./bin/server 8080
Start Clients

Open multiple terminals.

./bin/client

or

./bin/client 127.0.0.1 8080

Enter your username and start chatting.

Commands:

/quit  → disconnect from chat
📡 Communication Protocol
Step	Direction	Data
1	Client → Server	Username
2	Client → Server	Chat Message
3	Server → Clients	[username] message

Example:

[Satish]: Hello everyone!
[John]: Hi Satish!
🧠 Learning Outcomes

This project demonstrates:

Socket Programming

Multithreaded Server Design

Thread Synchronization

Concurrent Network Applications

🚀 Future Improvements

GUI chat interface

End-to-end encryption

Message history storage

File transfer support

Cross-platform threading

