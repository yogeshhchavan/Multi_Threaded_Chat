#ifndef CHATSERVER_H
#define CHATSERVER_H

#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>
#include <vector>
#include <string>

class ChatServer {
public:
    explicit ChatServer(int port);
    ~ChatServer();

    bool start();
    void stop();

private:
    void acceptClients();
    static DWORD WINAPI clientThreadFunc(LPVOID param);
    void handleClient(SOCKET clientSocket);
    void broadcastMessage(const std::string& message, SOCKET senderSocket);
    void removeClient(SOCKET clientSocket);

    int port_;
    SOCKET listenSocket_;
    std::vector<SOCKET> clients_;
    CRITICAL_SECTION clientsCS_;
    volatile bool running_;
};

// Parameter struct passed to the client thread
struct ClientThreadParam {
    ChatServer* server;
    SOCKET clientSocket;
};

#endif // CHATSERVER_H
