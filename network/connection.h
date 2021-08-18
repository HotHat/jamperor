//
// Created by admin on 2021/8/18.
//

#ifndef NETWORK_CONNECTION_H
#define NETWORK_CONNECTION_H

#include <cstdio>
#include <unistd.h>
#include <iostream>

namespace Network {

class Connection {
public:
    static const size_t kMaxBuffer;

    explicit Connection(int fd) { fd_ = fd; init(); }

    void init()
    {
        read_start_ = read_cur_ = read_buf_;
        read_end_ = read_buf_ + kMaxBuffer;
        write_buf_ = new char[kMaxBuffer];
        write_start_ = write_cur_ = write_end_ = write_buf_;
    }

    ~Connection() { delete write_buf_; }

    size_t AddWriteBuffer(const char *buffer, size_t size);

    void Write();
    ssize_t Read();

    void PrintReadBuffer() {
        std::cout << read_buf_ << std::endl;
    }

private:
    int fd_;
    // 2M buffer
    char read_buf_[1024 * 1024 * 2];
    // 2M buffer
    char *write_buf_;

    char *read_start_;
    char *read_end_;
    char *read_cur_;

    char *write_start_;
    char *write_end_;
    char *write_cur_;
};


}  // end namespace Network


#endif //NETWORK_CONNECTION_H
