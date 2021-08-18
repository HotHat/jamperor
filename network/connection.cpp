//
// Created by admin on 2021/8/18.
//

#include "connection.h"

const size_t Network::Connection::kMaxBuffer = 1024 * 1024 * 2;

void Network::Connection::Write() {
    ssize_t n;

    // do {
    n = ::write(fd_, write_cur_, write_end_ - write_cur_);
    // TODO: error handle
    write_cur_ += n;

    // } while (write_cur_ < write_end_);
}

ssize_t Network::Connection::Read() {
    ssize_t  n = 0;
    if (read_cur_ < read_end_) {
        n = ::read(fd_, read_cur_, read_end_ - read_cur_);
        read_cur_ += n;
    }

    return n;
}

size_t Network::Connection::AddWriteBuffer(const char *buffer, size_t size) {
    char *last = write_buf_ + kMaxBuffer;

    for (int i = 0; i < size; ++i)
    {
        // return write byte size
        if (write_end_ >= last) { return i;}
        *write_end_++ = buffer[i];
    }

    return size;
}
