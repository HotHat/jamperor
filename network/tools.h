//
// Created by Glyhux on 2021/8/17.
//

#ifndef NETWORK_TOOLS_H

#include <cstdio>
#include <cstdlib>

namespace Network {

    int guard(int n, const char *err) {
        if (n == -1) {
            perror(err);
            std::exit(1);
        }
        return n;
    }


} // namespace Network

#endif