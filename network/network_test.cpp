//
// Created by admin on 2021/8/17.
//

#include "socket.h"
#include <iostream>
#include "epoll.h"

int main(int argc, char* argv[])
{
    // std::cout << "hello world" << std::endl;
    Socket socket;

    Address address("47.98.198.35");

    socket.Connect(address, 80);

    Epoll epoll;

    epoll.Add(socket);

}