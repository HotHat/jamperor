//
// Created by admin on 2021/8/24.
//

#ifndef NETWORK_HTTP_RESPONSE_PARSE_H
#define NETWORK_HTTP_RESPONSE_PARSE_H
#include "buffer.h"
#include <functional>
#include <sstream>

namespace Network {

enum class StatusLineState {
    kStart,
    kHttp_H,
    kHttp_HH,
    kHttp_HHT,
    kHttp_HHTP,
    kHttpMajor,
    kHttpDot,
    kHttpMinor,
    kSpaceBeforeCode,
    kStatusCode_1,
    kStatusCode_2,
    kStatusCode_3,
    kSpaceBeforeReasonPhrase,
    kReasonPhrase,
    kAlmostDone
};

enum class ParseState {
    kOk,
    kError,
    kAgain,
    kEnd
};

class HttpResponseParse {
public:
    ParseState Parse();
    void SetBuffer(Buffer *buf) {
        buffer_ = buf;
    }
private:
    ParseState ParseStatusLine();

    void ParseHeader();

    void ParseHeaders();

    void ParseBody();

    // bool Compare(StatusLineState init, StatusLineState next, std::function<bool()> &compare);



    std::string reason_phrase_;
    int http_code_;
    StatusLineState status_line_state_;
    Network::Buffer *buffer_;
    std::stringstream ss;
};

} // end network namespace

#endif //NETWORK_HTTP_RESPONSE_PARSE_H
