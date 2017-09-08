cmake_minimum_required(VERSION 3.5)
project (TCPClient)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --std=c++11")

set(TCP_SOURCES src/TCPClient.cpp src/TCPClient.hpp)

add_library(tcpclient SHARED  ${TCP_SOURCES})

add_executable(test_client test/test_TCPClient.cpp)
target_link_libraries (test_client tcpclient -lpthread)
