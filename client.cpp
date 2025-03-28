#include <unistd.h>         // For close() system call
#include <sys/socket.h>     // For socket(), bind(), listen(), accept()
#include <arpa/inet.h>      // For inet_addr() and other networking functions
#include <iostream>         // For basic input/output
#include <cstring>
#include <vector>

using namespace std;

const int PORT = 8080;
const int BUFFER_SIZE = 1024;

int main()
{
    int socket_fd;
    struct sockaddr_in server_address;
    char buffer[BUFFER_SIZE];

    // create socket
    cout << "Creating client socket\n";
    socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_fd == -1){
        perror("Socket failed");
        exit(EXIT_FAILURE);
    }

    // configure server address and port struct
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(PORT);

    // Convert ipv4 address from text to binary format
    if(inet_pton(AF_INET, "127.0.0.1", &server_address.sin_addr) == -1){
        perror("Invalid address or address not supported");
        exit(EXIT_FAILURE);
    }

    //Connect to server
    cout << "Connecting to Server...\n";
    if (connect(socket_fd, (struct sockaddr *)&server_address, sizeof(server_address)) == -1){
        perror("Failed to connect to server");
        exit(EXIT_FAILURE);
    }

    cout << "Communicating with Server. Have fun!!\n";
    while(true){
        // take input from user
        // how does cin handle buffer overflow???
        cin >> buffer;

        // send input to server
        cout << "Sending message to server\n";

        // if client enters '/quit', break out of loop and close connection
        // need to add error handling when data exceeds buffer size
        if(send(socket_fd, buffer, BUFFER_SIZE, MSG_ERRQUEUE) == -1){ 
            perror("Error when sending data to server");
            break;
        }

        // recieve data from server
    }

    cout << "Closing connection to server!!!!!";
    close(socket_fd);
    exit(EXIT_SUCCESS);
}