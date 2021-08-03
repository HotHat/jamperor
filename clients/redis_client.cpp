//
// Created by admin on 2021/7/28.
//
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstdio>
#include <cstdlib>
#include <iostream>

#include "redis_resp.h"

#define MAX_LINE 1024

int main(int argc, char*argv[])
{
    std::cout << "Hello Cmake subproject world" << std::endl;

    int socket_fd;
    sockaddr_in server_address = {0};

    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(6379);
    server_address.sin_addr.s_addr = inet_addr("127.0.0.1");

    socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_fd < 0) {
        perror("socket");
        return EXIT_SUCCESS;
    }

    if(connect(socket_fd, reinterpret_cast<sockaddr *>(&server_address), sizeof(sockaddr_in)) == -1) {
        perror("connect");
        return EXIT_SUCCESS;
    }


    // std::string command("*2\r\n$4\r\nLLEN\r\n$6\r\nmylist\r\n");

    RespArray arr, arr2;
    auto auth = new RespBulkString("auth");
    auto password = new RespBulkString("NtoceahobN8287281Nre2");
    arr.Add(auth);
    arr.Add(password);
    std::string command = arr.ToString();

    char receive_line[MAX_LINE];
    send(socket_fd, command.c_str(), command.length(), 0);
    recv(socket_fd, receive_line, MAX_LINE, 0);
    std::cout << receive_line << std::endl;

    auto scan = new RespBulkString("scan");
    auto cursor = new RespBulkString("0");
    auto count = new RespBulkString("count");
    auto count_number = new RespBulkString("5");

    arr2.Add(scan);
    arr2.Add(cursor);
    arr2.Add(count);
    arr2.Add(count_number);
    std::string command2 = arr2.ToString();

    send(socket_fd, command2.c_str(), command2.length(), 0);
    auto cnt = recv(socket_fd, receive_line, MAX_LINE, 0);
    std::cout << "respond raw data: " <<  receive_line << std::endl;

    RedisResp resp;

    auto raw = std::string(receive_line, cnt);
    auto result = resp.parse(raw);

    if (result) {
        std::cout << "parser success" << std::endl;
        std::cout << result.value()->ToString() << std::endl;
    } else {
        std::cout << raw << "parser error" << std::endl;
    }





    return EXIT_SUCCESS;
}