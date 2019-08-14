cmake_minimum_required(VERSION 3.13)
project(code)

set(CMAKE_CXX_STANDARD 17)

add_executable(code main.cpp BinaryHeap.hpp SegmentTree.hpp Graphs.hpp MinMaxHeap.hpp GeneralAlgortihms.h SearchString.hpp)