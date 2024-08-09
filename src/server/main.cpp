#include "chatserver.hpp"
#include "chatservice.hpp"
#include <iostream>
#include <signal.h>

using namespace std;

void resetHander(int)
{
    ChatService::instance()->reset();
    exit(0);
}


int main(int argc, char **argv)
{
    //SIGINT 是 ctrl+c
    if(argc < 3)
    {
        cerr << "command invalid! example: ./ChatServer 127.0.0.1 8888" << endl;
    }
    char *ip = argv[1];
    uint16_t port = atoi(argv[2]);
    signal(SIGINT,resetHander);
    EventLoop loop;
    InetAddress addr(ip, port);
    ChatServer server(&loop, addr, "ChatServer");

    server.start();
    loop.loop();

    return 0;
}