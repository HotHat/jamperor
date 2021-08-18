//
// Created by admin on 2021/8/17.
//

#ifndef NETWORK_EPOLL_H
#define NETWORK_EPOLL_H
#include <sys/epoll.h>
#include "socket.h"

namespace Network {

class Epoll {
public:
    Epoll() : size_(512)
    {
        fd_ = epoll_create(size_);
    }

    Epoll(int size)
    {
        size_ = size;
        fd_ = epoll_create(size);
    }

    void Add(Socket &socket);
    void Delete(Socket &socket);
    void Modify(Socket &socket);
    void Wait();

private:
    int fd_;
    int size_;
};



} // end namespace Network

#endif //NETWORK_EPOLL_H
