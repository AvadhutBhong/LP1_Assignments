#include<iostream>
#include<cstring>
#include<netinet/in.h>
#include<sys/socket.h>
#include<unistd.h>

using namespace std;

int main(){

    int clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if(clientSocket < 0){
        cout<<"Failed to create socket"<<endl;
        return -1;
    }

    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(8080);
    serverAddress.sin_addr.s_addr = INADDR_ANY;

    if(connect(clientSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) < 0){
        cout<<"connection failed"<<endl;
        return -1;
    }

    cout<<"connected to server"<<endl;
    string message;
    getline(cin, message);
    send(clientSocket, message.c_str(), message.length(), 0);

    close(clientSocket);
    return 0;
}