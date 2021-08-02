//
// Created by admin on 2021/7/28.
//

#ifndef REDIS_CLIENT_REDIS_RESP_H
#define REDIS_CLIENT_REDIS_RESP_H
#include <vector>
#include <sstream>
#include <iostream>
#include <memory>
#include <stack>
#include <boost/optional.hpp>
#include <ctype.h>


class RespBase {
public:
    virtual std::string ToString() { return std::string(); };
};

class RespSimpleString : public RespBase{
public:
    RespSimpleString() = delete;

    explicit RespSimpleString(std::string str) : is_error_(false) {
        this->simple_string_ = std::move(str);
    }

    virtual std::string ToString() {
        std::stringstream ss;
        if (is_error_) {
            ss << "-" << simple_string_ << "\r\n";
        } else {
            ss << "+" << simple_string_ << "\r\n";
        }

        return std::string(ss.str());
    }

    bool IsError() {
        return is_error_;
    }

    void SetError(bool val) {
        is_error_ = val;
    }

private:

    bool is_error_;
    std::string simple_string_;
};

class RespBulkString : public RespBase{
public:
    RespBulkString() = delete;

    explicit RespBulkString(std::string str) : is_null_(false) {
        bulk_string_ = std::move(str);
    }

    std::string ToString() {
        if (is_null_) {
            return std::string("$-1\r\n");
        }

        std::stringstream ss;
        ss << "$" << bulk_string_.length() << "\r\n" << bulk_string_ << "\r\n";
        return std::string(ss.str());
    }

    static RespBulkString *Empty() {
        return new RespBulkString(std::string(""));
    }

    static RespBulkString *Null() {
        auto rb = new RespBulkString("");
        rb->SetNull(true);
        return rb;
    }

    bool IsNull() {
        return is_null_;
    }

    void SetNull(bool val) {
        is_null_ = val;
    }

private:


    bool is_null_;
    std::string bulk_string_;
};

class RespInteger : public RespBase{
public:
    RespInteger() = delete;

    explicit RespInteger(std::string str) {
        int_string_ = std::move(str);
    }

    explicit RespInteger(int str) {
        std::stringstream ss;
        ss << str;
        int_string_ = std::string(ss.str());
    }

    std::string ToString() {
        std::stringstream ss;
        ss << ":" << int_string_ << "\r\n";

        return std::string(ss.str());
    }

private:
    std::string int_string_;
};

class RespArray : public RespBase {
public:
    RespArray() : is_null_(false) {
        arr_ = std::vector<RespBase *>();
    }

    void Add(RespBase *resp) {
        arr_.push_back(resp);
    }

    std::string ToString() {
        if (is_null_) { return std::string("*-1\r\n"); }

        std::stringstream ss;
        ss << "*" << arr_.size() << "\r\n";
        for (auto &resp : arr_) {
//            std::cout << resp->ToString() << std::endl;
            ss << resp->ToString();
        }

        return std::string(ss.str());
    }

    static RespArray *Null() {
        auto rb = new RespArray();
        rb->SetNull(true);
        return rb;
    }

    bool IsNull() const {
        return is_null_;
    }

    size_t size() {
        return arr_.size();
    };

    void SetNull(bool val) {
        is_null_ = val;
    }

    ~RespArray() {
        for (auto s : arr_) {
            delete s;
        }
    }


private:
    bool is_null_;
    std::vector<RespBase *> arr_;

};

class RedisResp {
public:
    boost::optional<RespArray *> parse(std::string raw) {
        int32_t bulk_string_count = 0;
        int32_t array_count = 0;

        for ( std::string::iterator it = raw.begin(); it != raw.end(); ++it) {
            if (state_ == 0) {
                switch (*it) {
                    case '+': state_ = 1; break;
                    case '-': state_ = 2; break;
                    case ':': state_ = 3; break;
                    case '$': state_ = 4; bulk_string_count = 0; break;
                    case '*': state_ = 6; array_count = 0; break;
                    default: return opt_none;
                }
            } else if (state_ == 1 || state_ == 2 || state_ == 3) {
                if (*it == '\r') {
                    if (state_ == 1) {
                        state_ = 16;
                    } else if (state_ == 2) {
                        state_ = 26;
                    } else {
                        state_ = 36;
                    }
                } else {
                    AddBuffer(*it);
                }
            } else if (state_ == 16 || state_ == 26 || state_ == 36) {
                if (*it == '\n') {
                    auto buf = GetBuffer();
                    if (state_ == 16) {
                        AddRespObject(new RespSimpleString(buf));
                    } else if (state_ == 26) {
                        auto str = new RespSimpleString(buf);
                        str->SetError(true);
                        AddRespObject(str);
                    } else {
                        AddRespObject(new RespInteger(buf));
                    }
                } else {
                    return opt_none;
                }
            } else if (state_ == 4) {
                // number + \r\n
                if (*it == '-' || (std::isdigit(*it))) {
                    if (*it == '-') { state_ = 42; }
                    AddBuffer(*it);
                } else if (*it == '\r') {
                    state_ = 46;
                } else {
                    return opt_none;
                }
            } else if (state_ == 42) {
                if (*it == '1') {
                    AddBuffer('1');
                    state_ = 43;
                } else {
                    return opt_none;
                }
            }  else if (state_ == 43) {
                if (*it == '\r') {
                    state_ = 46;
                } else {
                    return opt_none;
                }

            } else if (state_ == 46) {
                if (*it == '\n') {
                    auto buf = GetBuffer();
                    bulk_string_count = std::stoi(buf);
                    if (bulk_string_count == -1) {
                        AddRespObject(RespBulkString::Null());
                    } else {
                        // reset to start
                        state_ = 50;
                    }

                } else {
                    return opt_none;
                }
            } else if (state_ == 50) {
                // std::cout << "ss:count:" << ss.tellp() << std::endl;
                if (GetBuffSize() < bulk_string_count) {
                    AddBuffer(*it);
                } else {
                    if (*it == '\r') {
                        state_ = 56;
                    } else {
                        return opt_none;
                    }
                }
            } else if (state_ == 56) {
                if (*it == '\n') {
                    auto buf = GetBuffer();
                    auto top = stack_.top();
                    auto str = new RespBulkString(buf);
                    top->Add(str);
                    ResetState();
                } else {
                    return opt_none;
                }
            } else if (state_ == 6) {
                if (*it == '-' || (std::isdigit(*it))) {
                    if (*it == '-') { state_ = 62; }
                    AddBuffer(*it);
                } else if (*it == '\r') {
                    state_ = 66;
                } else {
                    return opt_none;
                }
            } else if (state_ == 62) {
                if (*it == '1') {
                    AddBuffer('1');
                    state_ = 63;
                } else {
                    return opt_none;
                }
            } else if (state_ == 63) {
                if (*it == '\r') {
                    state_ = 66;
                } else {
                    return opt_none;
                }

            } else if (state_ == 66) {
                if (*it == '\n') {
                    auto buf = GetBuffer();
                    array_count = std::stoi(buf);
                    if (array_count == -1) {
                        AddRespObject(RespArray::Null());
                    } else {
                        stack_.push(new RespArray());
                        array_count_stack_.push(array_count);
                        ResetState();
                        // reset to start
                    }

                } else {
                    return opt_none;
                }
            }
        }

        if(state_ != 0 || stack_.size() != 1) { return opt_none; }

        auto resp = stack_.top();
        return boost::make_optional(resp);
    }

    explicit RedisResp() {
        // stack_ = std::stack<RespArray *>();
        // array_count_stack_ = std::stack<int>();
        stack_.push(new RespArray());
        // top level
        array_count_stack_.push(kTopArrayCount);
        opt_none = boost::optional<RespArray *>(boost::none);
        state_ = 0;
    }

    RedisResp(const RedisResp &) = delete;
    RedisResp(const RedisResp &&) = delete;

    ~RedisResp() {
        while (!stack_.empty()) {
            auto top = stack_.top();
            stack_.pop();
            delete top;
        }
    }

private:
    void AddBuffer(char ch) {
        ss_ << ch;
    }

    std::string GetBuffer() {
        auto s = ss_.str();
        ss_.str(std::string());
        return s;
    }

    void ResetState() {
        state_ = 0;
    }

    long GetBuffSize() {
        return ss_.tellp();
    }

    void AddRespObject(RespBase *obj) {
        auto top = stack_.top();
        auto stack_count = array_count_stack_.top();

        ResetState();
        // top level
        if (stack_count == kTopArrayCount) {
            top->Add(obj);
            return;
        }

        if (top->size() < stack_count) {
            top->Add(obj);
        }

        // pop up and add to stack
        if (top->size() == stack_count) {
            stack_.pop();
            array_count_stack_.pop();
            auto tp = stack_.top();
            tp->Add(top);
        }
    }


    const int kTopArrayCount = -1;
    int state_;
    std::stringstream ss_;
    std::stack<RespArray *> stack_;
    std::stack<int> array_count_stack_;
    boost::optional<RespArray *> opt_none;
};

boost::optional<std::shared_ptr<RespArray>> redis_resp_parse(std::string raw) {
    RespArray *resp_array = new RespArray();

    auto stack = std::stack<RespArray *>();
    stack.push(resp_array);
    // before first char 0
    // simple string 1
    // opt_none string 2
    // integer 3
    // bulk string 4
    // array 5
    // \r X6
    int state = 0;
    int32_t bulk_string_count = 0;
    std::stringstream ss;
    auto opt_none = boost::optional<std::shared_ptr<RespArray>>(boost::none);


    for ( std::string::iterator it = raw.begin(); it != raw.end(); ++it) {
        // std::cout << *it;
        if (state == 0) {
            switch (*it) {
                case '+': state = 1; break;
                case '-': state = 2; break;
                case ':': state = 3; break;
                case '$': state = 4; bulk_string_count = 0; break;
                case '*': state = 6;  break;
                default: return opt_none;
            }
        } else if (state == 1 || state == 2 || state == 3) {
            if (*it == '\r') {
                if (state == 1) {
                    state = 16;
                } else if (state == 2) {
                    state = 26;
                } else {
                    state = 36;
                }
            } else {
                ss << *it;
            }
        } else if (state == 16 || state == 26 || state == 36) {
            if (*it == '\n') {
                auto top = stack.top();
                auto buf = ss.str();
                ss.str(std::string());
                if (state == 16) {
                    auto str = new RespSimpleString(buf);
                    top->Add(str);
                } else if (state == 26) {
                    auto str = new RespSimpleString(buf);
                    str->SetError(true);
                    top->Add(str);
                } else {
                    auto str = new RespInteger(buf);
                    top->Add(str);
                }

                // reset to start
                state = 0;

            } else {
                return opt_none;
            }
        } else if (state == 4) {
            // number + \r\n
            if (*it == '-') {
                ss << *it;
                state = 42;
            } else if (std::isdigit(*it)) {
                ss << *it;
            } else if (*it == '\r') {
                state = 46;
            } else {
                return opt_none;
            }
        } else if (state == 42) {
            if (*it == '1') {
                ss << '1';
                state = 43;
            } else {
                return opt_none;
            }
        }  else if (state == 43) {
            if (*it == '\r') {
                state = 46;
            } else {
                return opt_none;
            }

        } else if (state == 46) {
            if (*it == '\n') {
                auto buf = ss.str();
                ss.str(std::string());
                bulk_string_count = std::stoi(buf);
                if (bulk_string_count == -1) {
                    auto top = stack.top();
                    auto str = RespBulkString::Null();
                    top->Add(str);
                    // reset to start
                    state = 0;
                } else {
                    // reset to start
                    state = 50;
                }

            } else {
                return opt_none;
            }
        } else if (state == 50) {
            // std::cout << "ss:count:" << ss.tellp() << std::endl;
            if (ss.tellp() < bulk_string_count) {
                ss << *it;
            } else {
                if (*it == '\r') {
                    state = 56;
                } else {
                    return opt_none;
                }
            }
        } else if (state == 56) {
            if (*it == '\n') {
                auto buf = ss.str();
                ss.str(std::string());
                auto top = stack.top();
                auto str = new RespBulkString(buf);
                top->Add(str);
                state = 0;
            } else {
                return opt_none;
            }
        }
    }

    if(state != 0) { return opt_none; }

    auto resp = std::shared_ptr<RespArray>(resp_array);
    return boost::make_optional(resp);
}

#endif //REDIS_CLIENT_REDIS_RESP_H
