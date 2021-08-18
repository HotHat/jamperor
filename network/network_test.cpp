//
// Created by admin on 2021/8/17.
//

// #include "socket.h"
#include "connection.h"
#include <iostream>
#include "epoll.h"

// using namespace Network;

int main(int argc, char* argv[])
{
    // std::cout << "hello world" << std::endl;
    Network::Socket socket;

    Address address("47.98.198.35");

    socket.Connect(address, 80);

    Network::Connection conn(socket.GetSocket());

    std::string message("GET / HTTP/1.1\r\nHost: release.quanyaotong.com\r\n\r\n");

    auto count = conn.AddWriteBuffer(message.c_str(), message.size());
    std::cout << "write count: " << count  << std::endl;

    conn.Write();

    socket.CloseWrite();

    ssize_t n;
    do {

        n = conn.Read();
        std::cout << "read count: " << n  << std::endl;

    } while (n > 0);

    conn.PrintReadBuffer();
}