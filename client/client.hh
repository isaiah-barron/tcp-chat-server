#ifndef CLIENT_HH
#define CLIENT_HH

#include <netinet/in.h>
#include <netdb.h>

class Client {
public:
    // Setup client state
    Client(ssize_t buff_size); //Constructor
    ~Client(); //Deconstructor
    
    void connect_to_server();
    void run();
    void close_client_socket();

private:
    // Static pointer to the current instance
    // we need this so we can call non-static 
    // methods in a static method
    static Client* instance;

    char *buffer;
    ssize_t BUFFER_SIZE;
    int socket_fd;
    struct addrinfo hints, *server_addr_info; // allows us to get host info of server

    // signal handling
    static void signal_handler(int s);
    void create_signal_handling();

    int read_user_input();
    void resize_buffer();

    // client messaging
    void send_message_length(ssize_t data_len);
    ssize_t send_message(const void *buf, size_t n);
    void send_all_data(ssize_t data_written);
};

#endif // CLIENT.HH