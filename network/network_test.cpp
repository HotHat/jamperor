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

    Network::Buffer buffer;
    std::string s = "HTTP/1.1 200 OK\r\n"
                    "Test.#: nginx\r\n"
                    "Pragma: no-cache\r\n\r\n";
    buffer.Add(s.c_str(), s.size());

    Network::HttpHeader http_header;

    Network::HttpResponseParse parse(&http_header, &buffer);
//    parse.SetBuffer(&buffer);
    if (parse.ParseStatusLine() != Network::ParseState::kOk) {
        std::cout << "parser status line fail" << std::endl;
    }
//     while (true) {
    Network::ParseState state;
    while (true) {
        state = parse.ParseHeader();
        if (state == Network::ParseState::kOk) {
            // parse header again

        }
        else if (state == Network::ParseState::kAgain) {
            // wait read more data

        }
        else if (state == Network::ParseState::kInvalidHeader) {
            // close connection
            break;

        } else if (state == Network::ParseState::kParseHeaderDone) {
            // parse http body
            break;
        }
    }

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