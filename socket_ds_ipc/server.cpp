#include <iostream>
#include <cstring>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>


using namespace std;


int main()
{
   // Create server socket
   int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
   if (serverSocket < 0) {
       cerr << "Failed to create socket." << endl;
       return -1;
   }


   // Specify server address and port
   sockaddr_in serverAddress;
   serverAddress.sin_family = AF_INET;
   serverAddress.sin_port = htons(8080);
   serverAddress.sin_addr.s_addr = INADDR_ANY;


   // Bind the socket to the specified IP and port
   if (bind(serverSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) < 0) {
       cerr << "Binding failed." << endl;
       close(serverSocket);
       return -1;
   }


   // Listen for incoming connections
   listen(serverSocket, 5);
   cout << "Server is listening on port 8080..." << endl;


   // Accept a single client connection (change to handle multiple clients, if needed)
   sockaddr_in clientAddress;
   socklen_t clientSize = sizeof(clientAddress);
   int clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddress, &clientSize);
   if (clientSocket < 0) {
       cerr << "Failed to accept client connection." << endl;
       close(serverSocket);
       return -1;
   }
   cout << "Client connected." << endl;


   // Continuously receive messages until the client sends "exit"
   char buffer[1024];
   while (true) {
       memset(buffer, 0, sizeof(buffer)); // Clear buffer
       int bytesReceived = recv(clientSocket, buffer, sizeof(buffer), 0);
       if (bytesReceived <= 0) {
           cerr << "Connection closed by client or error in receiving." << endl;
           break;
       }


       // Check for exit condition
       string receivedMessage(buffer);
       if (receivedMessage == "exit") {
           cout << "Client requested to close the connection." << endl;
           break;
       }


       // Display the received message
       cout << "Message from client: " << buffer << endl;
   }


   // Close the client socket and server socket
   close(clientSocket);
   close(serverSocket);
   cout << "Server shut down." << endl;


   return 0;
}
