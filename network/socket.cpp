//
// Created by admin on 2021/8/17.
//

#include "socket.h"


void Network::Socket::SetNonBlock() {
    int flags = guard(fcntl(socket_, F_GETFL), "could not get flags on TCP listening socket");
    guard(fcntl(socket_, F_SETFL, flags | O_NONBLOCK), "could not set TCP listening socket to be non-blocking");
}

void Network::Socket::CloseWrite() {
    guard(shutdown(socket_, SHUT_WR), "close write error");

}
