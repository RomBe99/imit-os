cmake_minimum_required(VERSION 3.15)
project(os)

set(CMAKE_CXX_STANDARD 20)

add_executable(client headers/daemon/msgdata.h src/daemon/msgdata.cpp src/daemon/Client.cpp)
add_executable(server headers/daemon/DaemonTools.h src/daemon/DaemonTools.cpp src/daemon/Server.cpp headers/daemon/msgdata.h src/daemon/msgdata.cpp)