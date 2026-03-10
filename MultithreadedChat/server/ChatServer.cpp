#include "ChatServer.h"
#include <iostream>
#include <cstring>
#include <algorithm>

static const int BUFFER_SIZE = 4096;
static const int MAX_USERNAME_LEN = 32;

ChatServer::ChatServer(int port)
    : port_(port), listenSocket_(INVALID_SOCKET), running_(false) {
    InitializeCriticalSection(&clientsCS_);
}

ChatServer::~ChatServer() {
    stop();
    DeleteCriticalSection(&clientsCS_);
}

bool ChatServer::start() {
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "[Server] WSAStartup failed.\n";
        return false;
    }

    listenSocket_ = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (listenSocket_ == INVALID_SOCKET) {
        std::cerr << "[Server] Failed to create socket.\n";
        return false;
    }

    // Allow port reuse
    int opt = 1;
    setsockopt(listenSocket_, SOL_SOCKET, SO_REUSEADDR,
               reinterpret_cast<const char*>(&opt), sizeof(opt));

    sockaddr_in serverAddr;
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(static_cast<u_short>(port_));

    if (bind(listenSocket_, reinterpret_cast<sockaddr*>(&serverAddr),
             sizeof(serverAddr)) == SOCKET_ERROR) {
        std::cerr << "[Server] Bind failed.\n";
        closesocket(listenSocket_);
        return false;
    }

    if (listen(listenSocket_, SOMAXCONN) == SOCKET_ERROR) {
        std::cerr << "[Server] Listen failed.\n";
        closesocket(listenSocket_);
        return false;
    }

    running_ = true;
    std::cout << "[Server] Listening on port " << port_ << "...\n";
    acceptClients();
    return true;
}

void ChatServer::stop() {
    running_ = false;

    if (listenSocket_ != INVALID_SOCKET) {
        closesocket(listenSocket_);
        listenSocket_ = INVALID_SOCKET;
    }

    EnterCriticalSection(&clientsCS_);
    for (size_t i = 0; i < clients_.size(); ++i) {
        closesocket(clients_[i]);
    }
    clients_.clear();
    LeaveCriticalSection(&clientsCS_);

    WSACleanup();
    std::cout << "[Server] Shut down.\n";
}

void ChatServer::acceptClients() {
    while (running_) {
        sockaddr_in clientAddr;
        int addrLen = sizeof(clientAddr);

        SOCKET clientSocket = accept(listenSocket_,
                                     reinterpret_cast<sockaddr*>(&clientAddr),
                                     &addrLen);
        if (clientSocket == INVALID_SOCKET) {
            if (running_) {
                std::cerr << "[Server] Accept failed.\n";
            }
            continue;
        }

        // Print client IP using inet_ntoa (available in older MinGW)
        std::cout << "[Server] New connection from "
                  << inet_ntoa(clientAddr.sin_addr)
                  << ":" << ntohs(clientAddr.sin_port) << "\n";

        EnterCriticalSection(&clientsCS_);
        clients_.push_back(clientSocket);
        LeaveCriticalSection(&clientsCS_);

        // Create thread param (freed inside the thread function)
        ClientThreadParam* param = new ClientThreadParam;
        param->server = this;
        param->clientSocket = clientSocket;

        HANDLE hThread = CreateThread(NULL, 0, clientThreadFunc, param, 0, NULL);
        if (hThread != NULL) {
            CloseHandle(hThread); // Detach — we don't need the handle
        }
    }
}

DWORD WINAPI ChatServer::clientThreadFunc(LPVOID param) {
    ClientThreadParam* p = static_cast<ClientThreadParam*>(param);
    p->server->handleClient(p->clientSocket);
    delete p;
    return 0;
}

void ChatServer::handleClient(SOCKET clientSocket) {
    // First message from client is their username
    char nameBuf[MAX_USERNAME_LEN + 1];
    memset(nameBuf, 0, sizeof(nameBuf));
    int nameLen = recv(clientSocket, nameBuf, MAX_USERNAME_LEN, 0);
    std::string username = "Unknown";
    if (nameLen > 0) {
        nameBuf[nameLen] = '\0';
        username = std::string(nameBuf);
        // Sanitize: keep only printable ASCII
        std::string clean;
        for (size_t i = 0; i < username.size(); ++i) {
            char c = username[i];
            if (c >= 32 && c <= 126) clean += c;
        }
        username = clean.empty() ? "Unknown" : clean;
    }

    std::string joinMsg = "[" + username + " joined the chat]\n";
    std::cout << joinMsg;
    broadcastMessage(joinMsg, clientSocket);

    char buffer[BUFFER_SIZE];
    while (running_) {
        int bytesReceived = recv(clientSocket, buffer, BUFFER_SIZE - 1, 0);
        if (bytesReceived <= 0) {
            break;
        }
        buffer[bytesReceived] = '\0';

        std::string message = "[" + username + "] " + std::string(buffer, bytesReceived);
        std::cout << message;
        broadcastMessage(message, clientSocket);
    }

    std::string leaveMsg = "[" + username + " left the chat]\n";
    std::cout << leaveMsg;
    broadcastMessage(leaveMsg, clientSocket);

    removeClient(clientSocket);
    closesocket(clientSocket);
}

void ChatServer::broadcastMessage(const std::string& message, SOCKET senderSocket) {
    EnterCriticalSection(&clientsCS_);
    for (size_t i = 0; i < clients_.size(); ++i) {
        if (clients_[i] != senderSocket) {
            send(clients_[i], message.c_str(), static_cast<int>(message.size()), 0);
        }
    }
    LeaveCriticalSection(&clientsCS_);
}

void ChatServer::removeClient(SOCKET clientSocket) {
    EnterCriticalSection(&clientsCS_);
    clients_.erase(
        std::remove(clients_.begin(), clients_.end(), clientSocket),
        clients_.end());
    LeaveCriticalSection(&clientsCS_);
}
