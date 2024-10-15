#include <iostream>
#include <cstring>          // For memset
#include <netinet/in.h>      // For sockaddr_in
#include <sys/socket.h>      // For socket functions
#include <unistd.h>          // For close()

using namespace std;

int main() {
    // Create a socket (AF_INET: IPv4, SOCK_STREAM: TCP)
    int clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket < 0) {
        cerr << "Failed to create socket." << endl;
        return -1;
    }

    // Specify the server's address and port number
    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;        // IPv4 address family
    serverAddress.sin_port = htons(8080);      // Port number in network byte order
    serverAddress.sin_addr.s_addr = INADDR_ANY; // Connect to localhost or any available IP

    // Connect to the server
    if (connect(clientSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) < 0) {
        cerr << "Connection failed." << endl;
        return -1;
    }

    cout << "Connected to server." << endl;

    // Loop to send messages to the server until the user types "exit"
    string message;
    while (true) {
        cout << "Enter your message (type 'exit' to quit): ";
        getline(cin, message);  // Read input from the user

        // Check if the user wants to exit
        if (message == "exit") {
            break;
        }

        // Send the message to the server
        send(clientSocket, message.c_str(), message.length(), 0);
    }

    // Close the socket once done
    close(clientSocket);
    cout << "Disconnected from server." << endl;

    return 0;
}
