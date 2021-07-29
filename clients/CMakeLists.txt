cmake_minimum_required(VERSION 3.16.0)
set(CMAKE_CXX_STANDARD  17)

project(redis_client)

find_package(Boost REQUIRED COMPONENTS
    unit_test_framework
)

add_executable(redis_client redis_client.cpp )


add_executable(test_resp redis_resp_test.cpp redis_resp.h)

target_link_libraries(test_resp
        ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
)
