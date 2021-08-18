//
// Created by admin on 2021/8/17.
//

#ifndef REDIS_CLIENT_SOCKET_H
#define REDIS_CLIENT_SOCKET_H

#include "address.h"
#include "tools.h"
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <iostream>
#include <unistd.h>
#include <fcntl.h>

namespace Network {

using namespace Network;

class Socket {
public:
    explicit Socket()
    {
        socket_ = guard(::socket(AF_INET, SOCK_STREAM, 0), "socket create failure");
        int flags = guard(fcntl(socket_, F_GETFL), "could not get flags on TCP listening socket");
        guard(fcntl(socket_, F_SETFL, flags | O_NONBLOCK), "could not set TCP listening socket to be non-blocking");
    }

    explicit Socket(int socket) { socket_ = socket; }

    int GetSocket() const { return socket_; }

    void SetOption();

    void Listen();

    void Bind();

    void Connect(Address &address, int port)
    {
        sockaddr_in socket_address = address.GetIpV4(port);

        int r = ::connect(socket_, reinterpret_cast<sockaddr *>(&socket_address), sizeof(sockaddr));
        guard(r, "connect fail");
    }

    ssize_t Write(std::string &message) const
    {
        return ::write(socket_, message.c_str(), message.size());
    }

private:
    int socket_;
};


#endif //REDIS_CLIENT_SOCKET_H

} // end namespace Network