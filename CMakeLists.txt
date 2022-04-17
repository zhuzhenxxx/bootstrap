cmake_minimum_required(VERSION 3.11)

project(bootstrap)

set(CMAKE_CXX_STANDARD 17)

add_executable(bootstrap main.cpp terp.h terp.cpp result_message.h result.h)