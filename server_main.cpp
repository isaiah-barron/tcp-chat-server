#include "server.hh"

int main()
{
    Server server(32);
    server.start();
    server.run();

    // Close server socket
    server.stop();

    return 0;
}
