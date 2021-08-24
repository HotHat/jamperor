//
// Created by admin on 2021/8/24.
//

#ifndef NETWORK_HTTP_H
#define NETWORK_HTTP_H
#include <string>
#include <map>

namespace Network {

struct HttpHeader {
    int major;
    int minor;
    int status_code;
    std::string reason_phrase;
    std::map<std::string, std::string> headers;

    bool is_valid_header;
};

}
#endif //NETWORK_HTTP_H
