#ifndef CHATCLIENT_H
#define CHATCLIENT_H

#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>
#include <string>

class ChatClient {
public:
    ChatClient(const std::string& serverIP, int port, const std::string& username);
    ~ChatClient();

    bool connectToServer();
    void run();
    void disconnect();

private:
    static DWORD WINAPI recvThreadFunc(LPVOID param);
    void receiveMessages();
    void sendMessages();

    std::string serverIP_;
    int port_;
    std::string username_;
    SOCKET socket_;
    volatile bool connected_;
};

#endif // CHATCLIENT_H
