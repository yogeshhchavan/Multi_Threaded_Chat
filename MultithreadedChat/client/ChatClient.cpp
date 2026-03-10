#include "ChatClient.h"
#include <iostream>
#include <cstring>

static const int BUFFER_SIZE = 4096;

ChatClient::ChatClient(const std::string& serverIP, int port, const std::string& username)
    : serverIP_(serverIP), port_(port), username_(username),
      socket_(INVALID_SOCKET), connected_(false) {}

ChatClient::~ChatClient() {
    disconnect();
}

bool ChatClient::connectToServer() {
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "[Client] WSAStartup failed.\n";
        return false;
    }

    socket_ = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (socket_ == INVALID_SOCKET) {
        std::cerr << "[Client] Failed to create socket.\n";
        return false;
    }

    sockaddr_in serverAddr;
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(static_cast<u_short>(port_));
    serverAddr.sin_addr.s_addr = inet_addr(serverIP_.c_str());

    if (serverAddr.sin_addr.s_addr == INADDR_NONE) {
        std::cerr << "[Client] Invalid server address: " << serverIP_ << "\n";
        closesocket(socket_);
        return false;
    }

    if (connect(socket_, reinterpret_cast<sockaddr*>(&serverAddr),
                sizeof(serverAddr)) == SOCKET_ERROR) {
        std::cerr << "[Client] Connection to server failed.\n";
        closesocket(socket_);
        return false;
    }

    // Send username as first message
    send(socket_, username_.c_str(), static_cast<int>(username_.size()), 0);

    connected_ = true;
    std::cout << "[Client] Connected to " << serverIP_ << ":" << port_
              << " as \"" << username_ << "\"\n";
    return true;
}

DWORD WINAPI ChatClient::recvThreadFunc(LPVOID param) {
    ChatClient* client = static_cast<ChatClient*>(param);
    client->receiveMessages();
    return 0;
}

void ChatClient::run() {
    if (!connected_) {
        std::cerr << "[Client] Not connected.\n";
        return;
    }

    // Start receive thread
    HANDLE hRecvThread = CreateThread(NULL, 0, recvThreadFunc, this, 0, NULL);

    // Main thread: read user input and send
    sendMessages();

    if (hRecvThread != NULL) {
        WaitForSingleObject(hRecvThread, 2000);
        CloseHandle(hRecvThread);
    }
}

void ChatClient::receiveMessages() {
    char buffer[BUFFER_SIZE];
    while (connected_) {
        int bytesReceived = recv(socket_, buffer, BUFFER_SIZE - 1, 0);
        if (bytesReceived <= 0) {
            if (connected_) {
                std::cout << "\n[Client] Disconnected from server.\n";
                connected_ = false;
            }
            break;
        }
        buffer[bytesReceived] = '\0';
        std::cout << std::string(buffer, bytesReceived);
        std::cout.flush();
    }
}

void ChatClient::sendMessages() {
    std::string message;
    while (connected_) {
        if (!std::getline(std::cin, message)) {
            break;
        }
        if (!connected_) break;

        if (message == "/quit") {
            std::cout << "[Client] Disconnecting...\n";
            break;
        }

        message += "\n";
        int result = send(socket_, message.c_str(),
                          static_cast<int>(message.size()), 0);
        if (result == SOCKET_ERROR) {
            std::cout << "[Client] Failed to send message.\n";
            break;
        }
    }
    disconnect();
}

void ChatClient::disconnect() {
    if (connected_) {
        connected_ = false;
    }
    if (socket_ != INVALID_SOCKET) {
        closesocket(socket_);
        socket_ = INVALID_SOCKET;
    }
    WSACleanup();
}
