# 🚀 Multithreaded Chat Application (C++)

A **real-time TCP chat application written in C++** that supports multiple simultaneous clients using **multithreading and socket programming**.

The server manages concurrent client connections and broadcasts messages between clients in real time.

---

# ✨ Features

- **Multi-client support**  
  Handles multiple clients concurrently using **Win32 threads (`CreateThread`)**

- **Real-time message broadcasting**  
  Messages from one client are instantly delivered to all other connected clients

- **Thread-safe architecture**  
  Shared client list protected using **`CRITICAL_SECTION`**

- **Graceful disconnect handling**  
  Server detects client disconnections and notifies remaining users

- **Username-based messaging**  
  Each client joins the chat using a unique username

---

# 🏗 System Architecture


Client 1 ─┐
Client 2 ─┼──────► Chat Server ─────► Broadcast to all clients
Client 3 ─┘


### Workflow

1. Server listens for incoming TCP connections  
2. Each client connection spawns a **separate thread**  
3. Client sends username after connecting  
4. Server receives messages and **broadcasts to all connected clients**  
5. When a client disconnects, the server removes it safely from the list  

---

# 📂 Project Structure

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

⚙️ Technologies Used
Technology	Purpose
C++	Core programming language
TCP Sockets	Network communication
Win32 Threads	Concurrent client handling
CRITICAL_SECTION	Thread synchronization


🛠 Build Instructions
Windows (MinGW / g++)
# Create output directory
mkdir bin

# Compile server
g++ -std=c++11 -Wall -o bin/server.exe server/main.cpp server/ChatServer.cpp -lws2_32

# Compile client
g++ -std=c++11 -Wall -o bin/client.exe client/main.cpp client/ChatClient.cpp -lws2_32

Linux / macOS
make

▶ Running the Application
Start the Server
./bin/server

or

./bin/server 8080

Start Clients

Open multiple terminals.

./bin/client

or

./bin/client 127.0.0.1 8080

Enter your username and start chatting.

Command to exit:

/quit

📡 Communication Protocol
Step	Direction	Data
1	Client → Server	Username
2	Client → Server	Chat Message
3	Server → Clients	[username] message

Example:

[Yogesh]: Hello everyone!
[John]: Hi Yogesh!

🧠 Learning Outcomes

This project demonstrates:

Socket Programming

Multithreaded Server Design

Thread Synchronization

Concurrent Network Applications

🚀 Future Improvements

GUI chat interface

End-to-end message encryption

Chat history persistence

File transfer capability

Cross-platform threading support

# 🏗 System Architecture
