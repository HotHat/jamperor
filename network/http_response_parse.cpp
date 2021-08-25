//
// Created by admin on 2021/8/24.
//

#include "http_response_parse.h"

Network::ParseState Network::HttpResponseParse::ParseStatusLine() {
    for (pos_ = buffer_->Begin(); pos_ < buffer_->End(); ++pos_) {
        byte_t ch = *pos_;

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
                http_header_->major = ch - '0';
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
                http_header_->minor = ch - '0';
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
                http_header_->status_code = http_code_;
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
                http_header_->reason_phrase = reason_phrase_;
                ss.str("");
                goto _done;
            } else {
                ss << ch;
            }
            continue;
        } else if (status_line_state_ == StatusLineState::kAlmostDone) {
            if (ch == '\n') {
                reason_phrase_ = ss.str();
                http_header_->reason_phrase = reason_phrase_;
                ss.str("");
                goto _done;
            }
        }

        return ParseState::kError;
    }

    return ParseState::kAgain;

_done:
    ++pos_;
    return ParseState::kOk;
}

Network::ParseState Network::HttpResponseParse::Parse() {
    auto r = ParseStatusLine();
    if (r != ParseState::kOk) { return  r; }

    return ParseHeader();
    // return Network::ParseState::kEnd;
}

Network::ParseState Network::HttpResponseParse::ParseHeader() {
    //the last '\0' is not needed because string is zero terminated
    // code from nginx source code
    static byte_t low_case[] =
            "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
            "\0\0\0\0\0\0\0\0\0\0\0\0\0-\0\0" "0123456789\0\0\0\0\0\0"
            "\0abcdefghijklmnopqrstuvwxyz\0\0\0\0\0"
            "\0abcdefghijklmnopqrstuvwxyz\0\0\0\0\0"
            "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
            "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
            "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
            "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0";

    byte_t c;
    std::string name, value;

    for (; pos_ < buffer_->End(); pos_++) {
        byte_t ch = *pos_;

        switch (header_state_) {
            // first chart
            case HeaderState::kStart: {
                header_name_start = pos_;

                if (ch == '\r') {
                    header_end = pos_;
                    header_state_ = HeaderState::kHeaderAlmostDone;
                } else if (ch == '\n') {
                    header_end = pos_;
                    goto _header_done;
                } else {
                    header_state_ = HeaderState::kName;
                    c = low_case[ch];

                    if (c) { break; }
                        // default support _, nginx should config on
                    else if (ch == '_') { break; }
                    else if (ch == '\0') { return ParseState::kInvalidHeader; }

                    is_valid_header_ = false;
                    break;
                }
            } break;
            // header name
            case HeaderState::kName: {
                c = low_case[ch];

                if (c) { break; }
                    // default support _, nginx should config on
                else if (ch == '_') { break; }
                else if (ch == ':') {
                    header_name_end = pos_;
                    header_state_ = HeaderState::kSpaceBeforeValue;
                    break;
                }
                else if (ch == '\r') {
                    header_name_end = header_start = header_end = pos_;
                    header_state_ = HeaderState::kAlmostDone;
                    break;
                } else if (ch == '\n') {
                    header_name_end = header_start = header_end = pos_;
                    goto _done;
                } else if (ch == '\0') { return ParseState::kInvalidHeader; }

                is_valid_header_ = false;
            } break;

            // space* before header value
            case HeaderState::kSpaceBeforeValue: {
                if (ch == ' ') { break; }
                else if (ch == '\r') {
                    header_start = header_end = pos_;
                    header_state_ = HeaderState::kAlmostDone;
                }
                else if (ch == '\n') {
                    header_start = header_end = pos_;
                    goto _header_done;
                }
                else if (ch == '\0') {
                    header_end = pos_;
                    return ParseState::kInvalidHeader;
                } else {
                    header_start = pos_;
                    header_state_ = HeaderState::kValue;
                }
            } break;

            // header value
            case HeaderState::kValue: {
                if (ch == ' ') {
                    header_end = pos_;
                    header_state_ = HeaderState::kSpaceAfterValue;
                }
                else if (ch == '\r') {
                    header_end = pos_;
                    header_state_ = HeaderState::kAlmostDone;
                }
                else if (ch == '\n') {
                    header_end = pos_;
                    goto _header_done;
                }
                else if (ch == '\0') {
                    header_end = pos_;
                    return ParseState::kInvalidHeader;
                }
            } break;

            // space* before end of header line
            case HeaderState::kSpaceAfterValue: {
                if (ch == ' ') { break; }
                else if (ch == '\r') { header_state_ = HeaderState::kAlmostDone; }
                else if (ch == '\n') { goto _header_done; }
                else if (ch == '\0') {
                    header_end = pos_;
                    return ParseState::kInvalidHeader;
                }
                else { header_state_ = HeaderState::kValue; }
            } break;

            // ignore header line
            case HeaderState::kIgnoreLine: {
                if (ch == '\n') { header_state_ = HeaderState::kStart; }
            } break;
            // end of header line
            case HeaderState::kAlmostDone: {
                if (ch == '\n') {
                    goto _done;
                } else if (ch == '\r') {
                    break;
                } else {
                    return ParseState::kInvalidHeader;
                }
            } break;

            // end of header
            case HeaderState::kHeaderAlmostDone: {
                if (ch == '\n') {
                    goto _header_done;
                } else {
                    return ParseState::kInvalidHeader;
                }
            } break;
        }
    }

    return ParseState::kAgain;

_done:
    ++pos_;
    header_state_ = HeaderState::kStart;
    // invalid header
    if (!is_valid_header_) {
        is_valid_header_ = true;
    } else {
        for (auto i = header_name_start; i < header_name_end; ++i) { ss << *i; }
        name = ss.str();
        ss.str("");

        for (auto i = header_start; i < header_end; ++i) { ss << *i; }
        value = ss.str();
        ss.str("");
        http_header_->headers[name] = value;
    }

    return ParseState::kOk;

_header_done:
    ++pos_;
    header_state_ = HeaderState::kStart;
    return ParseState::kParseHeaderDone;

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


Network::HttpResponseParse::HttpResponseParse(Network::HttpHeader *http_header, Network::Buffer *buffer) {
    http_header_ = http_header;
    buffer_ = buffer;
}

Network::ParseState Network::HttpResponseParse::ParseChunk() {

    return Network::ParseState::kOk;
}
