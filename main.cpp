#include <sys/types.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include <iostream>


void pa(const char *arr, int count) {
    for(int i = 0; i < count; ++i) {
//        std::cout << static_cast<int>(arr[i]) << std::endl;
        std::cout << arr[i] << " ";
    }
    std::cout << std::endl;
}

int main() {
    const int max_events = 10;
    const int buf_len = 10;

    socklen_t address_len = sizeof(sockaddr_in);

    int server_fd, client_fd;
    sockaddr_in server_address = {0}, client_address = {0};

    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(7080);
    server_address.sin_addr.s_addr = inet_addr("127.0.0.1");


    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    const int opt = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    if(bind(server_fd, (sockaddr *)&server_address, address_len) == -1) {
        perror("bind error");
    }

    if(listen(server_fd, 6) == -1) {
        perror("listen error");
    }


    int efd, nfds;
    epoll_event ev;
    epoll_event events[max_events];
    efd = epoll_create1(0);

    ev.events = EPOLLIN;
    ev.data.fd = server_fd;

    epoll_ctl(efd, EPOLL_CTL_ADD, server_fd, &ev);
    char buf[buf_len] = {0};

    while (true) {
        nfds = epoll_wait(efd, events, max_events, 0);

        for(int i = 0; i < nfds ; ++i) {
            int event = events[i].events;
            int current_fd = events[i].data.fd;

            if (current_fd == server_fd) {
                client_fd = accept(server_fd, (sockaddr *)&client_address, &address_len);
                ev.events = EPOLLIN | EPOLLRDHUP;
                ev.data.fd = client_fd;
                epoll_ctl(efd, EPOLL_CTL_ADD, client_fd, &ev);

            } else {

                if (event & EPOLLHUP) {
                    std::cout << "epoll hup happened!" << std::endl;
                    ::close(current_fd);
                    epoll_ctl(efd, EPOLL_CTL_DEL, client_fd, &ev);
                } else if (event & EPOLLRDHUP) {
                    std::cout << "epoll rdhup happened!" << std::endl;
                    ::close(current_fd);
                    epoll_ctl(efd, EPOLL_CTL_DEL, client_fd, &ev);

                } else if (event & EPOLLIN) {
                    std::cout << "epoll in happened! event: " << event << std::endl;
                    size_t n = read(current_fd, buf, buf_len);
                    std::cout << "size of read: " << n << std::endl;
                    pa(buf, n);
                    write(current_fd, buf, n);

                } else if (event & EPOLLOUT) {
                    std::cout << "epoll out happened!" << std::endl;
                    write(current_fd, "epoll out happened!", 19);
                }

            }
        }
    }


    std::cout << "Hello, World!" << std::endl;
    return 0;
}