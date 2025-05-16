#include <iostream>
#include <unistd.h>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <thread>
#include <vector>
#include <atomic>

using namespace std;

constexpr int PORT = 8080;

ssize_t recvFull(int sockfd, const void* buf, size_t len) {
    size_t total = 0;
    ssize_t sent = 0;
    const char* ptr = static_cast<const char*>(buf);

    // only send the data we havent sent yet up to len
    while (total < len) {
        sent = send(sockfd, ptr + total, len - total, 0);
        if (sent <= 0) return sent;
        total += sent;
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

    while (true) {
        cout << "Client connected...\n";

        uint32_t net_len;
        ssize_t r = recvFull(client_fd, &net_len, sizeof(net_len));
        if (r <= 0) {
            std::cout << "Client disconnected or error\n";
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

        // since client sends us msg len before we read the actual data
        // we create a buffer with that exact size, thus we need to delete the buffer
        // once we are done reading the message so we can create a new buffer
        // for the next message
        delete[] buffer;
    }

    close(client_fd);
    close(server_fd);
    return 0;
}
