cmake_minimum_required(VERSION 3.16.0)
set(CMAKE_CXX_STANDARD  17)

project(network)

add_library(network STATIC
        socket.cpp
        connection.cpp
        epoll.cpp
        buffer.cpp
        http_response_parse.cpp
)


add_executable(network_test network_test.cpp)

target_link_libraries(network_test
        network
)

