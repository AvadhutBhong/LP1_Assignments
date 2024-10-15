#include <iostream>
#include <cstring>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>


using namespace std;


int main()
{
   // Create socket
   int clientSocket = socket(AF_INET, SOCK_STREAM, 0);
   if (clientSocket < 0) {
       cerr << "Failed to create socket." << endl;
       return -1;
   }


   // Specify server address
   sockaddr_in serverAddress;
   serverAddress.sin_family = AF_INET;
   serverAddress.sin_port = htons(8080);
   serverAddress.sin_addr.s_addr = INADDR_ANY;


   // Connect to server
   if (connect(clientSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) < 0) {
       cerr << "Connection failed." << endl;
       return -1;
   }


   cout << "Connected to server." << endl;


   // Keep sending messages until user types 'exit'
   string message;
   while (true) {
       cout << "Enter your message (type 'exit' to quit): ";
       getline(cin, message);


       // Check for exit condition
       if (message == "exit") {
           break;
       }


       // Send message to server
       send(clientSocket, message.c_str(), message.length(), 0);
   }


   // Close socket
   close(clientSocket);
   cout << "Disconnected from server." << endl;


   return 0;
}
