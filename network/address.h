//
// Created by admin on 2021/8/17.
//

#ifndef REDIS_CLIENT_ADDRESS_H
#define REDIS_CLIENT_ADDRESS_H
#include <string>
#include <netinet/in.h>
#include <arpa/inet.h>

class Address {
public:
    explicit Address(std::string address) { address_ = std::move(address); }

    sockaddr_in GetIpV4(int port) {
        sockaddr_in socket_address{0};

        socket_address.sin_family = AF_INET;
        socket_address.sin_port = htons(port);
        socket_address.sin_addr.s_addr = inet_addr(address_.c_str());

        return socket_address;
    }

private:
    std::string address_;
};


#endif //REDIS_CLIENT_ADDRESS_H
