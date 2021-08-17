cmake_minimum_required(VERSION 3.16.0)

project(jamperor)

add_definitions( -DBOOST_LOG_DYN_LINK=1 )

set(CMAKE_CXX_STANDARD 17)
find_package(Boost REQUIRED COMPONENTS
    regex
    filesystem
    system
    log_setup
    log
    thread
    unit_test_framework
)


add_executable(jamperor main.cpp logger.h clients/redis_client.cpp)
target_link_libraries(jamperor
        ${Boost_LOG_SETUP_LIBRARY}
        ${Boost_LOG_LIBRARY}
        Boost::thread
        )

add_executable(tester boost_unit_test.cpp)


target_link_libraries(tester
        ${Boost_FILESYSTEM_LIBRARY}
        ${Boost_SYSTEM_LIBRARY}
        ${Boost_LOG_SETUP_LIBRARY}
        ${Boost_LOG_LIBRARY}
        ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
        Boost::thread
        )

add_subdirectory(clients)
add_subdirectory(network)

