cmake_minimum_required(VERSION 3.13)
project(cp)

set(CMAKE_CXX_STANDARD 17)

add_executable(cp stacks/queueusingtwostacks.cpp)
target_link_libraries(cp m)