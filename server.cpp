#include <unistd.h>         // For close() system call
#include <cstring>
#include <sys/socket.h>     // For socket(), bind(), listen(), accept()
#include <string.h>         // Basic string library functions
#include <arpa/inet.h>      // For inet_addr() and other networking functions
#include <iostream>         // For basic input/output
#include <thread>
#include <mutex>
#include <vector>

using namespace std;

const int PORT = 8080;
const int BUFFER_SIZE = 1024;

void handleClient(int conn_fd)
{
    char buffer[BUFFER_SIZE];

    printf("Client connected...");
    while (true){
        // wait to recv message from client
        // server send confirmation message to client
        // need to add handling of when client disconnects
        // need to add handling when theres a send or recv error (-1)
            // do we close the connection or can the server send a reset and have the client reconnect if it recvs a reset from server?
        
        // place holder
        break;
    }

    close(conn_fd);
}

int start_server(sockaddr_in address)
{
    // Create server socket using IPv4 and socket type TCP 
    int socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_fd == -1){
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // We need to bind the socket to an ip address and port
    address.sin_family = AF_INET; // This specifies the address family as IPv4
    address.sin_addr.s_addr = INADDR_ANY; // This will bind to all available interfaces
    address.sin_port = htons(PORT); // coverts port number to network byte order

    if (bind(socket_fd, (sockaddr*)&address, sizeof(address)) == -1){
        perror("Bind failed");
        close(socket_fd);
        exit(EXIT_FAILURE);
    }

    // Server will listen for connections
    if (listen(socket_fd, 10)){
        perror("Listen failed");
        close(socket_fd);
        exit(EXIT_FAILURE);
    }

    return socket_fd;
}

int main()
{
    int server_socket, client_socket;
    sockaddr_in server_address, client_address; // This struct is used to store information about the socket address
    socklen_t cli_addr_len = sizeof(client_address);
    vector<thread> threads; // to keep track of each thread created when the server handles a client

    // Start server
    server_socket = start_server(server_address);

    // Server waits to accept new connections
    while (true){
        // We wait for a connection from a client
        printf("Server waiting for a client to connect....");
        if((client_socket = accept(server_socket, (sockaddr*)&client_address, &cli_addr_len)) == -1){
            perror("Accept failed");
            close(server_socket);
            exit(EXIT_FAILURE);
        }

        // we create a new thread to handle the client
        threads.push_back(thread(handleClient, client_socket));
    }

    // Clean up threads
    for (auto& t : threads){
        if(t.joinable()) t.join();
    }

    // Close server socket
    close(server_socket);
    exit(EXIT_SUCCESS);
}
