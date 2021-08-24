//
// Created by admin on 2021/8/17.
//

// #include "socket.h"
#include "connection.h"
#include <iostream>
#include "epoll.h"
#include "http_response_parse.h"

// using namespace Network;

int main(int argc, char* argv[])
{
    Network::HttpResponseParse parse;

    Network::Buffer buffer;
    std::string s = "HTTP/1.1 302 Found\r\n";
    buffer.Add(s.c_str(), s.size());

    parse.SetBuffer(&buffer);
    auto state = parse.Parse();

    std::cout << static_cast<std::underlying_type<Network::ParseState>::type>(state) << std::endl;


    // std::cout << "hello world" << std::endl;
    // Network::Socket socket;
    //
    // Address address("47.98.198.35");
    //
    // socket.Connect(address, 80);
    //
    // Network::Connection conn(socket.GetSocket());
    //
    // std::string message("GET / HTTP/1.1\r\nHost: release.quanyaotong.com\r\n\r\n");
    //
    // auto count = conn.AddWriteBuffer(message.c_str(), message.size());
    // std::cout << "write count: " << count  << std::endl;
    //
    // conn.Write();
    //
    // // socket.CloseWrite();
    // // socket.SetNonBlock();
    //
    // ssize_t n;
    // do {
    //
    //     n = conn.Read();
    //     std::cout << "read count: " << n  << std::endl;
    //
    // } while (n > 0);
    //
    // conn.PrintReadBuffer();
}