#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include "ODSocket.h"

int main(int argc, char *argv[])
{
    ODSocket sock;
    char buf[1024];
    sock.Create(AF_INET, SOCK_STREAM, 0);
    sock.Connect("127.0.0.1", 4321);
    int len = sock.Recv(buf, 1024);
    buf[len] = 0;
    std::cout<< buf << std::endl;
    return 0;
}
