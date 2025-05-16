#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netinet/in.h>
#include <netdb.h>

using namespace std;

constexpr int PORT = 8080;

ssize_t sendFull(int sockfd, const void* buf, size_t len) {
    size_t total = 0;
    ssize_t sent = 0;

    // we have to cast it because we cant do
    // ptr arithmitec (or dereference) with a generic pointer (void).
    // we are not modifying the buffer, so we make it const
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
    // initialize hints struct
    struct addrinfo hints, *server_addr_info; // allows us to get host info of server
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;

    // get hostname info from addrinfo struct
    int res = getaddrinfo("tcp_server", "8080", &hints, &server_addr_info);
    if (res){
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(res));
        exit(EXIT_FAILURE);
    }

    //create socket
    int socket_fd = socket(server_addr_info->ai_family, 
        server_addr_info->ai_socktype, 
        server_addr_info->ai_protocol);

    if (socket_fd == -1){
        perror("Socket failed");
        exit(EXIT_FAILURE);
    }

    if (connect(socket_fd, server_addr_info->ai_addr, server_addr_info->ai_addrlen) < 0) {
        perror("Connect failed");
        close(socket_fd);
        exit(EXIT_FAILURE);
    }

    string line;
    while (true) {
        cout << "Enter message (/quit to exit): ";
        getline(cin, line);
        if (line == "/quit") break;

        uint32_t len = line.size();
        uint32_t net_len = htonl(len);

        // send message length
        if (sendFull(socket_fd, &net_len, sizeof(net_len)) <= 0) {
            cerr << "Send length failed\n";
            break;
        }

        // send message body
        if (sendFull(socket_fd, line.data(), len) <= 0) {
            cerr << "Send message failed\n";
            break;
        }
    }

    close(socket_fd);
    return 0;
}
