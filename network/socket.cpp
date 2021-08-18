//
// Created by admin on 2021/8/17.
//

#include "socket.h"


void Network::Socket::SetOption() {

}

void Network::Socket::CloseWrite() {
    guard(shutdown(socket_, SHUT_WR), "close write error");

}
