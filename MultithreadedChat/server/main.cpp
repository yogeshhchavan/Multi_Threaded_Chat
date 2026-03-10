#include "ChatServer.h"
#include <iostream>
#include <cstdlib>

int main(int argc, char* argv[]) {
    int port = 54000; // default port
    if (argc >= 2) {
        port = std::atoi(argv[1]);
        if (port <= 0 || port > 65535) {
            std::cerr << "Invalid port number. Using default 54000.\n";
            port = 54000;
        }
    }

    ChatServer server(port);
    if (!server.start()) {
        std::cerr << "Failed to start server.\n";
        return 1;
    }

    return 0;
}
