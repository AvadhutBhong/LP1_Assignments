#include<iostream>
#include<cstring>
#include<netinet/in.h>
#include<sys/socket.h>
#include<unistd.h>

using namespace std;

int main(){

    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if(serverSocket < 0){
        cout<<"Failed to create socket"<<endl;
        return -1;
    }

    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(8080);
    serverAddress.sin_addr.s_addr = INADDR_ANY;

    if(bind(serverSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) < 0){
        cout<<"connection failed"<<endl;
        return -1;
    }

    listen(serverSocket, 5);

    sockaddr_in clientAddress;
    socklen_in clientsize = sizeof(clientAddress);
    int clientSocket = accept(serverSocket, (struct sockaddr*) &clientAddress, &clientsize );
    if(clientSocket < 0){
        cout<<"failed to accept client connection"<<endl;
        return -1;
        close(serverSocket);
    }

    char buffer[1024];
    while(true){
        memset(buffer, 0 , sizeof(buffer));
        int byteReveived = recv(clientSocket, buffer, sizeof(buffer) , 0);
        if(byteReveived <= 0){
            cout<<'connection closed'<<endl;
            break;
        }

        string receiveMessage(buffer);
        if(receiveMessage =="exit"){
            cout<<"client requested to close"<<endl;
            break;
        }
        cout<<buffer<<endl;
    }
    close(clientSocket);
    close(serverSocket);

    return 0;


}