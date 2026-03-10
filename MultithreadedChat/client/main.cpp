#include "ChatClient.h"
#include <iostream>
#include <cstdlib>

int main(int argc, char* argv[]) {
    std::string serverIP = "127.0.0.1";
    int port = 54000;

    if (argc >= 2) serverIP = argv[1];
    if (argc >= 3) {
        port = std::atoi(argv[2]);
        if (port <= 0 || port > 65535) {
            std::cerr << "Invalid port number. Using default 54000.\n";
            port = 54000;
        }
    }

    std::string username;
    std::cout << "Enter your username: ";
    std::getline(std::cin, username);
    if (username.empty()) {
        username = "Anonymous";
    }
    // Truncate to 32 chars
    if (username.size() > 32) {
        username = username.substr(0, 32);
    }

    ChatClient client(serverIP, port, username);
    if (!client.connectToServer()) {
        std::cerr << "Failed to connect to server.\n";
        return 1;
    }

    std::cout << "Type messages and press Enter to send. Type /quit to exit.\n";
    client.run();

    return 0;
}
