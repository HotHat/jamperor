//
// Created by admin on 2021/8/17.
//

#ifndef REDIS_CLIENT_SOCKET_H
#define REDIS_CLIENT_SOCKET_H

#include "address.h"
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <boost/assert.hpp>
#include <iostream>
#include <unistd.h>


class Socket {
public:
    explicit Socket()
    {
        socket_ = ::socket(AF_INET, SOCK_STREAM, 0);
    }

    explicit Socket(int socket) { socket_ = socket; }

    void SetOption();

    void Listen();

    void Bind();

    void Connect(Address &address, int port)
    {
        sockaddr_in socket_address = address.GetIpV4(port);

        int r = ::connect(socket_, reinterpret_cast<sockaddr *>(&socket_address), sizeof(sockaddr));
        BOOST_ASSERT_MSG(r != -1, "connect fail");
    }

    ssize_t Write(std::string &message)
    {
        return ::write(socket_, message.c_str(), message.size());
    }

private:
    int socket_;
};


#endif //REDIS_CLIENT_SOCKET_H
