#include <iostream>
#include <cstring>          // For memset
#include <netinet/in.h>      // For sockaddr_in
#include <sys/socket.h>      // For socket functions
#include <unistd.h>          // For close()
#include <thread>            // For multithreading
#include <vector>            // To store threads for multiple clients

using namespace std;

// Function to handle each client's communication in a separate thread
void handleClient(int clientSocket) {
    char buffer[1024];  // Buffer to receive messages

    // Continuously receive messages from the client
    while (true) {
        memset(buffer, 0, sizeof(buffer));  // Clear the buffer before receiving data
        int bytesReceived = recv(clientSocket, buffer, sizeof(buffer), 0);  // Receive message

        // If no data received or client disconnected, exit the loop
        if (bytesReceived <= 0) {
            cerr << "Client disconnected or error receiving data." << endl;
            break;
        }

        // Display the received message on the server console
        cout << "Message from client: " << buffer << endl;

        // Send an acknowledgment back to the client
        string response = "Server: Message received!";
        send(clientSocket, response.c_str(), response.length(), 0);
    }

    // Close the client socket when done
    close(clientSocket);
    cout << "Client socket closed." << endl;
}

int main() {
    // Create a TCP socket (AF_INET: IPv4, SOCK_STREAM: TCP)
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket < 0) {
        cerr << "Failed to create socket." << endl;
        return -1;
    }

    // Define the server address and port number
    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;        // IPv4 address family
    serverAddress.sin_port = htons(8080);      // Port number in network byte order
    serverAddress.sin_addr.s_addr = INADDR_ANY; // Listen on all network interfaces

    // Bind the socket to the specified address and port
    if (bind(serverSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) < 0) {
        cerr << "Binding failed." << endl;
        close(serverSocket);
        return -1;
    }

    // Start listening for incoming connections, with a backlog of 5
    listen(serverSocket, 5);
    cout << "Server is listening on port 8080..." << endl;

    // Vector to store threads handling each client
    vector<thread> threads;

    // Accept connections from clients in an infinite loop
    while (true) {
        sockaddr_in clientAddress;  // To store the client's address
        socklen_t clientSize = sizeof(clientAddress);  // Size of client address structure

        // Accept a new client connection
        int clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddress, &clientSize);
        if (clientSocket < 0) {
            cerr << "Failed to accept client connection." << endl;
            continue;  // Continue to accept other clients
        }

        cout << "Client connected!" << endl;

        // Create a new thread to handle the client's communication
        threads.emplace_back(handleClient, clientSocket);
    }

    // Optional: Wait for all threads to finish (if needed for cleanup)
    for (auto& t : threads) {
        if (t.joinable()) {
            t.join();  // Wait for the thread to finish execution
        }
    }

    // Close the server socket when done
    close(serverSocket);
    cout << "Server shut down." << endl;

    return 0;
}
