#include <unistd.h>         // For close() system call
#include <sys/socket.h>     // For socket(), bind(), listen(), accept()
#include <arpa/inet.h>      // For inet_addr() and other networking functions
#include <signal.h>
#include <iostream>         // For basic input/output
#include <cstring>
#include <vector>

using namespace std;

const int PORT = 8080;
ssize_t BUFFER_SIZE = 32;
char *buffer = new char[BUFFER_SIZE];
int socket_fd = 0;

void close_client_socket()
{
    cout << "Closing connection to server!!!!!";
    close(socket_fd);
    exit(EXIT_SUCCESS);
}

void my_handler(int s)
{
    printf("Caught signal %d\n", s);
    close_client_socket();
}

void create_signal_handling()
{
    struct sigaction sigIntHandler;

    sigIntHandler.sa_handler = my_handler;
    sigemptyset(&sigIntHandler.sa_mask);
    sigIntHandler.sa_flags = 0;

    sigaction(SIGINT, &sigIntHandler, NULL);
}

int main()
{
    struct sockaddr_in server_address;
    char bytes_to_send[32];

    // signal handling
    create_signal_handling();

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
        cin.getline(buffer, BUFFER_SIZE);

        if (strcmp(buffer, "/quit") == 0){
            break;
        }

        // we need to resize buffer if user input exceeds the buffer size
        if (cin.gcount() == BUFFER_SIZE - 1 && strlen(buffer) == BUFFER_SIZE - 1){
            cout << "we need to rezie the buffer!?!?!?!\n";
            // 1) Allocate a new larger buffer
            BUFFER_SIZE = BUFFER_SIZE * 2;
            char *tmp_buff = new char[BUFFER_SIZE];
            char leftover[BUFFER_SIZE];

            // 2) Copy the original buffer into the new one
            strcpy(tmp_buff, buffer);

            // 3) clear any cin errors
            cin.clear();

            // 4) grab the leftover input and append to the new buffer
            cin.getline(leftover, BUFFER_SIZE);
            strcat(tmp_buff, leftover);

            // 5) delete allocated memory of buffer and 
            //    make buffer pointer point to new tmp buffer
            delete[] buffer;
            buffer = tmp_buff;
        }

        // send a message to the server about how much data we want to send
        // send 'data written', the amount of data we want to send to the server
        ssize_t data_written = strlen(buffer);
        snprintf(bytes_to_send, sizeof(bytes_to_send), "%zd", data_written);

        if (send(socket_fd, bytes_to_send, sizeof(bytes_to_send), 0) == -1){ 
            perror("Error when sending data to server");
            exit(EXIT_FAILURE);
        }

        // send input to server
        cout << "Sending message to server\n";
        ssize_t total_sent = 0;
        while(total_sent < data_written)
        {
            ssize_t sent = send(socket_fd, buffer, data_written, 0);
            if (sent == -1){ 
                perror("Error when sending data to server");
                exit(EXIT_FAILURE);
            }

            total_sent += sent;
            cout << "how much data sent:\n" << total_sent << "\n";
        }

        // recieve data from server???
        // maybe an ack message that data was received
    }

    close_client_socket();
}