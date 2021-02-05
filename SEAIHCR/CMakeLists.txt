cmake_minimum_required(VERSION 3.17)
project(SEAIHCR)

set(CMAKE_CXX_STANDARD 17)

add_executable(SEAIHCR main.cpp Compartment.cpp Compartment.h Model.cpp Model.h Distribution.cpp Distribution.h prob.cpp prob.hpp)