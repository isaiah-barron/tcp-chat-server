#include "client.hh"

int main()
{
    Client client(32);
    client.connect_to_server();
    client.run();

    return 0;
}