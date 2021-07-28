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


    std::string command("*2\r\n$4\r\nLLEN\r\n$6\r\nmylist\r\n");

    send(socket_fd, command.c_str(), command.length(), 0);
    char receive_line[MAX_LINE];
    recv(socket_fd, receive_line, MAX_LINE, 0);

    std::cout << receive_line << std::endl;





    return EXIT_SUCCESS;
}