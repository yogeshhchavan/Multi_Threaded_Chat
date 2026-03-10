# Makefile for MultithreadedChat
# Works on Linux/macOS with g++. For Windows, use the PowerShell build commands in README.md.

CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra -pthread

# Output directories
BIN_DIR = bin

# Source files
SERVER_SRC = server/main.cpp server/ChatServer.cpp
CLIENT_SRC = client/main.cpp client/ChatClient.cpp

# Targets
SERVER_BIN = $(BIN_DIR)/server
CLIENT_BIN = $(BIN_DIR)/client

.PHONY: all clean server client

all: server client

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

server: $(BIN_DIR)
	$(CXX) $(CXXFLAGS) -o $(SERVER_BIN) $(SERVER_SRC)

client: $(BIN_DIR)
	$(CXX) $(CXXFLAGS) -o $(CLIENT_BIN) $(CLIENT_SRC)

clean:
	rm -rf $(BIN_DIR)
