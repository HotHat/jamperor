//
// Created by admin on 2021/7/28.
//

#ifndef REDIS_CLIENT_REDIS_RESP_H
#define REDIS_CLIENT_REDIS_RESP_H
#include <vector>

class RedisResp {

};

class RedisRespBase {
public:
    str::string ToString();
};

class RespSimpleString : public RedisRespBase{
public:
    explicit RespSimpleString(std::string str) {
        this->simple_string_ = str;
    }

    std::string ToString() {
        return std::string("+" + this->simple_string_);
    }

    std::string Error(std::string &prefix = "ERR") {
        return std::string("-" + prefix + " " + this->simple_string_);
    }

private:
    std::string simple_string_;
};

class RespBulkString : public RedisRespBase{

};

class RespInteger : public RedisRespBase{

};

class RespArray : public RedisRespBase {

private:
    std::vector<RedisRespBase> arr_;

};

#endif //REDIS_CLIENT_REDIS_RESP_H
