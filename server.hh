#ifndef SERVER_HH
#define SERVER_HH

#include <netinet/in.h>
#include <netdb.h>

class Server{
public:

    //constructor desctructor
    Server(ssize_t buff_size);
    ~Server();

private:

    char *buffer;
    ssize_t BUFFER_SIZE;
    int conn_fd;

    
};

#endif // SERVER.hh