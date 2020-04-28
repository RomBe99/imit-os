cmake_minimum_required(VERSION 3.15)
project(os)

set(CMAKE_CXX_STANDARD 20)

add_executable(os src/main.cpp headers/file-io-tasks/apue.h headers/file-io-tasks/file_with_hole.h headers/copy_file_test/CopyFileUtils.h headers/cmd/CmdUtils.h src/copy_file_test/CopyFileUtils.cpp headers/Runners.h src/cmd/CmdUtils.cpp)