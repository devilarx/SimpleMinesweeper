cmake_minimum_required(VERSION 3.16)
project(SimpleMinesweeper VERSION 0.01)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(SimpleMinesweeper main.cpp Core/Field.cpp Core/Field.h)