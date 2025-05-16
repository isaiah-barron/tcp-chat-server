#include <iostream>
#include <unistd.h>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <vector>

using namespace std;

constexpr int PORT = 8080;

ssize_t recvFull(int sockfd, void* buf, size_t len) {
    size_t total = 0;
    ssize_t recieved = 0;

    // we have to cast it because we cant do
    // ptr arithmitec (or dereference) with a generic pointer (void)
    char* ptr = static_cast<char*>(buf);

    // only send the data we havent sent yet up to len
    while (total < len) {
        recieved = recv(sockfd, ptr + total, len - total, 0);
        if (recieved <= 0) return recieved;
        total += recieved;
    }
    return total;
}

int main() {
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    addr.sin_addr.s_addr = INADDR_ANY;

    bind(server_fd, (sockaddr*)&addr, sizeof(addr));
    listen(server_fd, 1);

    cout << "Server listening on port " << PORT << endl;

    int client_fd = accept(server_fd, nullptr, nullptr);
    if (client_fd < 0) {
        perror("Accept failed");
        return 1;
    }

    cout << "Client connected...\n";
    while (true) {

        uint32_t net_len;
        ssize_t r = recvFull(client_fd, &net_len, sizeof(net_len));
        if (r <= 0) {
            cout << "Client disconnected or error\n";
            break;
        }

        uint32_t msg_len = ntohl(net_len); //convert network byte to unsigned integer 32
        if (msg_len == 0) continue; // no message was sent, go back to top of loop and wait for nxt msg

        // create new buffer with buffer size of messge length
        char* buffer = new char[msg_len + 1];
        r = recvFull(client_fd, buffer, msg_len); // recv message from client
        if (r <= 0) {
            delete[] buffer;
            cout << "Client disconnected or error\n";
            break;
        }

        buffer[msg_len] = '\0'; // null terminate

        cout << "Received message: " << buffer << endl;

        // Delete buffer for the next message
        delete[] buffer;
    }

    close(client_fd);
    close(server_fd);
    return 0;
}
