cmake_minimum_required(VERSION 3.16.0)
set(CMAKE_CXX_STANDARD  17)

project(redis_client)

add_executable(redis_client redis_client.cpp redis_resp.cpp redis_resp.h)