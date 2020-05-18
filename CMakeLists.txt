cmake_minimum_required(VERSION 3.15)
project(os)

set(CMAKE_CXX_STANDARD 20)

# Для компиляции клиента необходимо раскомментировать строку с Reader.h/cpp и в Reader.cpp раскомментировать main
# Для компиляции сервера необходимо раскомментировать строку с Writer.h/cpp и в Writer.cpp раскомментировать main

add_executable(os headers/daemon/msgdata.h src/daemon/msgdata.cpp
#        headers/daemon/Reader.h src/daemon/Reader.cpp
#        headers/daemon/Daemon.h src/daemon/Daemon.cpp headers/daemon/Writer.h src/daemon/Writer.cpp
        )