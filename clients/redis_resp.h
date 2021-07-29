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


class RedisRespBase {
public:
    virtual std::string ToString() { return std::string(); };
};

class RespSimpleString : public RedisRespBase{
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

class RespBulkString : public RedisRespBase{
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

    static RespBulkString Empty() {
        return RespBulkString(std::string(""));
    }

    static RespBulkString Null() {
        auto rb = RespBulkString("");
        rb.SetNull();
        return rb;
    }

private:
    void SetNull() {
        is_null_ = true;
    }

    bool is_null_;
    std::string bulk_string_;
};

class RespInteger : public RedisRespBase{
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

class RespArray : public RedisRespBase {
public:
    RespArray() : is_null_(false) {
        arr_ = std::vector<RedisRespBase *>();
    }

    void Add(RedisRespBase *resp) {
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

    static RespArray Null() {
        auto rb = RespArray();
        rb.SetNull();
        return rb;
    }


private:
    void SetNull() {
        is_null_ = true;
    }

    bool is_null_;
    std::vector<RedisRespBase *> arr_;

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
    std::stringstream ss;
    auto opt_none = boost::optional<std::shared_ptr<RespArray>>(boost::none);


    for ( std::string::iterator it = raw.begin(); it != raw.end(); ++it) {
        std::cout << *it;
        if (state == 0) {
            switch (*it) {
                case '+': state = 1; break;
                case '-': state = 2; break;
                case ':': state = 3; break;
                case '$': state = 4; break;
                case '*': state = 5;  break;
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
                if (state == 16) {
                    auto str = new RespSimpleString(ss.str());
                    top->Add(str);
                } else if (state == 26) {
                    auto str = new RespSimpleString(ss.str());
                    str->SetError(true);
                    top->Add(str);
                } else {
                    auto str = new RespInteger(ss.str());
                    top->Add(str);
                }
            } else {
                return opt_none;
            }
        }
    }

    if(state == 0) { return opt_none; }

    auto resp = std::shared_ptr<RespArray>(resp_array);
    return boost::make_optional(resp);
}

#endif //REDIS_CLIENT_REDIS_RESP_H
