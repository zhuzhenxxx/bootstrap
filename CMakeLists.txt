cmake_minimum_required(VERSION 3.11)

project(bootstrap)

set(CMAKE_CXX_STANDARD 17)
include_directories(../fmt/include/)
link_directories(../fmt/build/)
link_libraries(fmtd)
add_executable(bootstrap main.cpp terp.h terp.cpp result_message.h result.h)
target_link_libraries(bootstrap libfmtd.a)