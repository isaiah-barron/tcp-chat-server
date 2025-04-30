#ifndef SERVER_HH
#define SERVER_HH

#include <netinet/in.h>
#include <netdb.h>
#include <thread>
#include <atomic>
#include <vector>

class Server{
public:
    Server(ssize_t buff_size);
    ~Server(); // set stop_flag to true and we will need to clean up threads here

    void start();
    void stop();
    void run();
    
private:
    
    int server_socket;
    sockaddr_in server_address, client_address;
    char *buffer;
    ssize_t BUFFER_SIZE;
    std::vector<std::thread> threads;
    std::atomic<bool> stop_flag; //allows us to terminate threads when server class is out of scope
    
    void handleClient(int conn_fd);
    int acceptConnection();
    void recieveMessage();
};

#endif // SERVER.hh