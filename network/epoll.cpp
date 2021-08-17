//
// Created by admin on 2021/8/17.
//

#include "epoll.h"

void Network::Epoll::Add(Socket &socket)
{

    epoll_event event{};
    event.events = EPOLLIN | EPOLLOUT | EPOLLRDHUP | EPOLLET;
    event.data.fd = socket.GetSocket();

    Network::guard(epoll_ctl(fd_, EPOLL_CTL_ADD, socket.GetSocket(), &event),
                   "can't call epoll_ctl add");

}

void Network::Epoll::Wait()
{

}
