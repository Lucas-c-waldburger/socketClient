cmake_minimum_required(VERSION 3.24)
project(socketClient)

set(CMAKE_CXX_STANDARD 17)

add_executable(socketClient main.cpp includes.h Utils.cpp Utils.h)
