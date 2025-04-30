#include "server.hh"
#include <unistd.h>
#include <cstring>
#include <sys/socket.h>
#include <string.h>
#include <arpa/inet.h>
#include <iostream>
#include <thread>
#include <mutex>
#include <vector>

using namespace std;

const char PORT = '8080';

Server::Server(ssize_t buff_size)
{
    BUFFER_SIZE = buff_size;
    buffer = new char[buff_size];

    // Create server socket using IPv4 and socket type TCP 
    cout << "Creating Server socket\n";
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1){
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // We need to bind the socket to an ip address and port
    server_address.sin_family = AF_INET; // This specifies the address family as IPv4
    server_address.sin_addr.s_addr = INADDR_ANY; // This will bind to all available interfaces
    server_address.sin_port = htons(PORT); // coverts port number to network byte order
}

Server::~Server()
{
    // Tell all threads to shutdown
    // store is thread safe
    stop_flag.store(true);

    // Clean up threads
    for (auto& t : threads){
        if(t.joinable()) t.join();
    }
}

void Server::start()
{
    cout << "Binding server socket to ip address and port\n";
    if (::bind(server_socket, (sockaddr*)&server_address, sizeof(server_address)) == -1){
        perror("Bind failed");
        close(server_socket);
        exit(EXIT_FAILURE);
    }

    // Server will listen for connections
    cout << "Setting socket to listen for connections\n\n";
    if (listen(server_socket, 10)){
        perror("Listen failed");
        close(server_socket);
        exit(EXIT_FAILURE);
    }
}

void Server::stop()
{
    close(server_socket);
    exit(EXIT_SUCCESS);
}

void Server::run()
{
    int client_socket;
    // Server waits to accept new connections
    while (true){
        // We wait for a connection from a client
        client_socket = acceptConnection();

        // we create a new thread to handle the client
        // **note use emplace_back bc it constructs the thread in-place directly inside the vector 
        // and does not require creating a tmp object and then copy.
        // in order for threads to run the non-static memmber method handle_client from
        // the Server class, we have to provide a pointer to the method and provide the current instnace 
        // of the class Server
        threads.emplace_back(&Server::handleClient, this, client_socket);
    }
}

int Server::acceptConnection()
{
    cout << "Server waiting for a client to connect....\n\n";

    socklen_t cli_addr_len = sizeof(client_address);
    int conn_fd = accept(server_socket, (sockaddr*)&client_address, &cli_addr_len);

    if(conn_fd == -1){
        perror("Accept failed");
        close(server_socket);
        exit(EXIT_FAILURE);
    }

    return conn_fd;
}

void Server::handleClient(int conn_fd)
{
    cout << "Client connected...\n";
    cout << "SERVICING CLIENT....\n";

    // While stop_flag is false, thread continoues
    // to service the client. load() makes reading stop_flag thread safe
    while (!stop_flag.load()){
        // wait to recv message from client
        cout << "waiting for message.\n\n";

        // TODO: 
        // 1) Error handling
            // need interrupt hanlding for ctrl c
            // need to add handling for when client sends '/quit'
            // need to add handling of when client disconnects
            // need to add handling when theres a send or recv error (-1)
                // do we close the connection or the server send a reset 
                // and have the client reconnect if it recvs a reset from server?
        // 2) need to handle error when buffer size is exceeded
            // or see if remaining data stays in recv and we can retrieve it

        if(recv(conn_fd, buffer, BUFFER_SIZE, 0) == -1){ 
            perror("Error in receiving data from client");
            // exit(EXIT_FAILURE);
        }
        cout << buffer << '\n';

        // 3) server send confirmation message to client
    }

    close(conn_fd);
}