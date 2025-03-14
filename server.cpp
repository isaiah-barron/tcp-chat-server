#include <iostream>         // For basic input/output
#include <unistd.h>         // For close() system call
#include <sys/socket.h>     // For socket(), bind(), listen(), accept()
#include <netinet/in.h>     // For sockaddr_in structure and htons()
#include <string.h>         // Basic string library functions
#include <arpa/inet.h>      // For inet_addr() and other networking functions
#include <stdlib.h>         // C++ standard library

int main()
{
    const int PORT = 8080;

    // Create server socket using IPv4 and socket type TCP 
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1){
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // We need to bind the socket to an ip address and port
    struct sockaddr_in address; // This struct is used to store information about the socket address
    address.sin_family = AF_INET; // This specifies the address family as IPv4
    address.sin_addr.s_addr = INADDR_ANY; // This will bind to all available interfaces
    address.sin_port - htons(PORT); // coverts port number to network byte order

    if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) == -1){
        perror("Bind failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }
}
