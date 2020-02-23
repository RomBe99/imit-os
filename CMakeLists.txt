cmake_minimum_required(VERSION 3.15)
project(os)

set(CMAKE_CXX_STANDARD 20)

add_executable(os src/Main.cpp headers/file-io-tasks/apue.h)