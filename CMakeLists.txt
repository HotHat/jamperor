cmake_minimum_required(VERSION 3.16.0)

project(jamperor)

set(CMAKE_CXX_STANDARD 17)
find_package(Boost REQUIRED COMPONENTS
    regex
    filesystem
    system
    log
    thread
    unit_test_framework
)


add_executable(jamperor main.cpp)

add_executable(tester boost_unit_test.cpp)

target_link_libraries(tester
        ${Boost_FILESYSTEM_LIBRARY}
        ${Boost_SYSTEM_LIBRARY}
        ${Boost_LOG_LIBRARY}
        ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
        Boost::thread
        )


