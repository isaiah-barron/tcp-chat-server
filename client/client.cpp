#include "client.hh"  // Include the header file to use class declarations
#include <iostream>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <signal.h>

using namespace std;

// define instance
Client* Client::instance = nullptr; 

//Contructor Definition
Client::Client(ssize_t buff_size) {
    instance = this;

    BUFFER_SIZE = buff_size;
    buffer = new char[buff_size];

    //initialize hints struct
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;

    // get hostname info from addrinfo struct
    int res = getaddrinfo("tcp_server", "8080", &hints, &server_addr_info);
    if (res){
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(res));
        exit(EXIT_FAILURE);
    }

    //create socket
    socket_fd = socket(server_addr_info->ai_family, 
                       server_addr_info->ai_socktype, 
                       server_addr_info->ai_protocol);
    if (socket_fd == -1){
        perror("Socket failed");
        exit(EXIT_FAILURE);
    }

    create_signal_handling();
}

void Client::signal_handler(int s)
{
    printf("Caught signal %d\n", s);
    instance->close_client_socket();
}

void Client::create_signal_handling()
{
    struct sigaction sigIntHandler;

    sigIntHandler.sa_handler = signal_handler;
    sigemptyset(&sigIntHandler.sa_mask);
    sigIntHandler.sa_flags = 0;

    sigaction(SIGINT, &sigIntHandler, NULL);
}

void Client::connect_to_server()
{
    //Connect to server
    cout << "Connecting to Server...\n";
    if (connect(socket_fd, server_addr_info->ai_addr, server_addr_info->ai_addrlen) == -1){
        perror("Failed to connect to server");
        exit(EXIT_FAILURE);
    }
}

int Client::read_user_input()
{
    cin.getline(buffer, BUFFER_SIZE);

    if (strcmp(buffer, "/quit") == 0){
        return -1;
    }

    if (cin.gcount() == BUFFER_SIZE - 1 && static_cast<ssize_t>(strlen(buffer)) == BUFFER_SIZE - 1){
        resize_buffer();
    }

    return 0;
}

void Client::resize_buffer()
{
    cout << "Resizing buffer\n";

    // clear any cin errors
    cin.clear();

    // we need to check how much data is remaining in the stream to get new BUFF size
    string leftover;
    while(cin.peek() != '\n'){
        leftover.push_back(cin.get());
    }
    
    BUFFER_SIZE = BUFFER_SIZE + leftover.length();
    cout << "new buffer size: " << BUFFER_SIZE << "\n";

    char *tmp_buff = new char[BUFFER_SIZE];

    // Copy the original buffer into the new one
    strcpy(tmp_buff, buffer);

    // grab the leftover input and append to the new buffer
    strcat(tmp_buff, leftover.data());

    // make buffer pointer points to new tmp buffer
    delete[] buffer;
    buffer = tmp_buff;
}

void Client::send_message_length(ssize_t data_len)
{
    char bytes_to_send[20];

    // send a message to the server about how much data we want to send
    // send 'data written', the amount of data we want to send to the server
    snprintf(bytes_to_send, sizeof(bytes_to_send), "%zd", data_len);

    send_message(bytes_to_send, sizeof(bytes_to_send));
}

ssize_t Client::send_message(const void *buf, size_t n)
{
    cout << "Sending message to server\n";
    ssize_t sent = send(socket_fd, buf, n, 0);
    if (sent == -1){ 
        perror("Error when sending data to server");
        exit(EXIT_FAILURE);
    }

    return sent;
}

void Client::send_all_data(ssize_t data_written)
{
    ssize_t total_sent = 0;
    ssize_t sent = 0;
    while(total_sent < data_written)
    {
        sent = send_message(buffer + total_sent, data_written - total_sent);

        total_sent += sent;
        cout << "how much data sent:\n" << total_sent << "\n";
    }
}

void Client::run()
{
    cout << "Communicating with Server. Have fun!!\n";
    while(true){
        // take input from user
        if (read_user_input() == -1){
            break;
        }

        ssize_t data_written = strlen(buffer);
        send_message_length(data_written);

        // send data to server
        send_all_data(data_written);

        // recieve data from server???
        // maybe an ack message that data was received
    }
}

void Client::close_client_socket()
{
    // Send FIN before client closes its socket
    char *fin = "FIN";
    if (send(socket_fd, fin, sizeof(fin), 0) == -1){ 
        perror("Error when sending data to server");
        exit(EXIT_FAILURE);
    }
    cout << "Sending FIN packet to server!!!!";

    delete [] buffer;
    close(socket_fd);
    
    exit(EXIT_SUCCESS);
}

// Destructor Definition
Client::~Client() {
    instance = nullptr;
    if (socket_fd != -1){
        close_client_socket();
    }
}