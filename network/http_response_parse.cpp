//
// Created by admin on 2021/8/24.
//

#include "http_response_parse.h"

Network::ParseState Network::HttpResponseParse::ParseStatusLine() {

    for (auto i = buffer_->Begin(); i < buffer_->End(); ++i) {
        byte_t ch = *i;
        
        if (status_line_state_ == StatusLineState::kStart) {
            if (ch == 'H') {
                status_line_state_ = StatusLineState::kHttp_H;
                continue;
            }
        } else if (status_line_state_ == StatusLineState::kHttp_H) {
            if (ch == 'T') {
                status_line_state_ = StatusLineState::kHttp_HH;
                continue;
            }
        } else if (status_line_state_ == StatusLineState::kHttp_HH) {
            if (ch == 'T') {
                status_line_state_ = StatusLineState::kHttp_HHT;
                continue;
            }
        } else if (status_line_state_ == StatusLineState::kHttp_HHT) {
            if (ch == 'P') {
                status_line_state_ = StatusLineState::kHttp_HHTP;
                continue;
            }
        } else if (status_line_state_ == StatusLineState::kHttp_HHTP) {
            if (ch == '/') {
                status_line_state_ = StatusLineState::kHttpMajor;
                continue;
            }
        } else if (status_line_state_ == StatusLineState::kHttpMajor) {
            if (ch >= '0' && ch <= '9') {
                status_line_state_ = StatusLineState::kHttpDot;
                continue;
            }
        } else if (status_line_state_ == StatusLineState::kHttpDot) {
            if (ch == '.') {
                status_line_state_ = StatusLineState::kHttpMinor;
                continue;
            }
        }
        else if (status_line_state_ == StatusLineState::kHttpMinor) {
            if (ch >= '0' && ch <= '9') {
                status_line_state_ = StatusLineState::kSpaceBeforeCode;
                continue;
            }
        } else if (status_line_state_ == StatusLineState::kSpaceBeforeCode) {
            if (ch >= ' ') {
                status_line_state_ = StatusLineState::kStatusCode_1;
                continue;
            }
        } else if (status_line_state_ == StatusLineState::kStatusCode_1) {
            if (ch >= '0' && ch <= '9') {
                http_code_ = http_code_ * 10 + (ch - '0');
                status_line_state_ = StatusLineState::kStatusCode_2;
                continue;
            }
        } else if (status_line_state_ == StatusLineState::kStatusCode_2) {
            if (ch >= '0' && ch <= '9') {
                http_code_ = http_code_ * 10 + (ch - '0');
                status_line_state_ = StatusLineState::kStatusCode_3;
                continue;
            }
        } else if (status_line_state_ == StatusLineState::kStatusCode_3) {
            if (ch >= '0' && ch <= '9') {
                http_code_ = http_code_ * 10 + (ch - '0');
                status_line_state_ = StatusLineState::kSpaceBeforeReasonPhrase;
                continue;
            }
        } else if (status_line_state_ == StatusLineState::kSpaceBeforeReasonPhrase) {
            if (ch >= ' ') {
                status_line_state_ = StatusLineState::kReasonPhrase;
                continue;
            }
        } else if (status_line_state_ == StatusLineState::kReasonPhrase) {
            if (ch == '\r') {
                status_line_state_ = StatusLineState::kAlmostDone;
                continue;
            } else if(ch == '\n') {
                reason_phrase_ = ss.str();
                return ParseState::kOk;
            } else {
                ss << ch;
            }
            continue;
        } else if (status_line_state_ == StatusLineState::kAlmostDone) {
            if (ch == '\n') {
                reason_phrase_ = ss.str();
                return ParseState::kOk;
            }
        }

        return ParseState::kError;
    }

    return ParseState::kAgain;
}

Network::ParseState Network::HttpResponseParse::Parse() {
    return ParseStatusLine();
    // return Network::ParseState::kEnd;
}

// bool Network::HttpResponseParse::Compare(Network::StatusLineState init, Network::StatusLineState next,
//                                          std::function<bool()> &compare) {
//     if (status_line_state_ == init) {
//         if (compare()) {
//             status_line_state_ = next;
//             return true;
//         }
//     }
//     return false;
// }


