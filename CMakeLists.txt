cmake_minimum_required(VERSION 3.11)

project(Pentago CXX)

set(CMAKE_CXX_STANDARD 14)

add_subdirectory(external)

enable_testing()
add_subdirectory(src)
