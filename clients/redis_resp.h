//
// Created by admin on 2021/7/28.
//

#ifndef REDIS_CLIENT_REDIS_RESP_H
#define REDIS_CLIENT_REDIS_RESP_H
#include <vector>
#include <sstream>
#include <iostream>
#include <memory>

class RedisResp {

};

class RedisRespBase {
public:
    virtual std::string ToString() { return std::string(); };
};

class RespSimpleString : public RedisRespBase{
public:
    RespSimpleString() = delete;

    explicit RespSimpleString(std::string str) {
        this->simple_string_ = std::move(str);
    }

    virtual std::string ToString() {
        std::stringstream ss;
        ss << "+" << simple_string_ << "\n\n";

        return std::string(ss.str());
    }

    std::string Error(std::string prefix = "ERR") {
        std::stringstream ss;
        ss << "-" << prefix << " " << simple_string_ << "\n\n";
        return std::string(ss.str());
    }

private:
    std::string simple_string_;
};

class RespBulkString : public RedisRespBase{
public:
    RespBulkString() = delete;

    explicit RespBulkString(std::string str) : is_nil_(false) {
        bulk_string_ = std::move(str);
    }

    std::string ToString() {
        if (is_nil_) {
            return std::string("$-1\r\n");
        }

        std::stringstream ss;
        ss << "$" << bulk_string_.length() << "\r\n" << bulk_string_ << "\n\n";
        return std::string(ss.str());
    }

    static RespBulkString Empty() {
        return RespBulkString(std::string(""));
    }

    static RespBulkString Nil() {
        auto rb = RespBulkString("");
        rb.SetNil();
        return rb;
    }

private:
    void SetNil() {
        is_nil_ = true;
    }

    bool is_nil_;
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
        ss << ":" << int_string_ << "\n\n";

        return std::string(ss.str());
    }

private:
    std::string int_string_;
};

class RespArray : public RedisRespBase {
public:
    RespArray() {
        arr_ = std::vector<std::shared_ptr<RedisRespBase>>();
    }

    void Add(std::shared_ptr<RedisRespBase> resp) {
        arr_.push_back(resp);
    }

    std::string ToString() {
        std::stringstream ss;
        ss << "*" << arr_.size() << "\r\n";
        for (auto &resp : arr_) {
//            std::cout << resp->ToString() << std::endl;
            ss << resp->ToString();
        }

        return std::string(ss.str());
    }


private:
    std::vector<std::shared_ptr<RedisRespBase>> arr_;

};

#endif //REDIS_CLIENT_REDIS_RESP_H
