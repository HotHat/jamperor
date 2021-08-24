//
// Created by admin on 2021/8/24.
//

#ifndef NETWORK_BUFFER_H
#define NETWORK_BUFFER_H
// #include <stddef.h>
#include <cstddef>

#define BUFFER_MAX_SIZE 1024 * 1024 * 2
typedef unsigned char byte_t;

namespace Network {

class Buffer {
public:
    Buffer() { Reset(); }
    size_t Add(const char *buffer, size_t size);

    size_t Size() { return current_ - buffer_; }

    byte_t Current() { if (end_ == buffer_) { return '\0'; } return *current_; }

    void Reset();
    byte_t *Begin() { return buffer_; }
    byte_t *End() { return current_; }

private:
    byte_t *current_;
    byte_t *end_;
    byte_t buffer_[BUFFER_MAX_SIZE];
};

} // end network namespace
#endif //NETWORK_BUFFER_H
