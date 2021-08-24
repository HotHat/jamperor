//
// Created by admin on 2021/8/24.
//

#include "buffer.h"

size_t Network::Buffer::Add(const char *buffer, size_t size) {
    byte_t *last = buffer_ + sizeof(buffer_);

    for (int i = 0; i < size; ++i)
    {
        // return write byte size
        if (current_ >= last) { return i;}

        *current_++ = buffer[i];
    }

    return size;
}

void Network::Buffer::Reset() {
    current_ = buffer_;
}
