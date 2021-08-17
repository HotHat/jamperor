//
// Created by admin on 2021/8/17.
//

#ifndef REDIS_CLIENT_EPOLL_H
#define REDIS_CLIENT_EPOLL_H
#include <sys/epoll.h>

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

    void Add();
    void Delete();
    void Modify();
    void Wait();

private:
    int fd_;
    int size_;
};


#endif //REDIS_CLIENT_EPOLL_H
