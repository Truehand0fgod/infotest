cmake_minimum_required(VERSION 3.20)

project(MyLib)

add_library(mylib SHARED mylib.cpp mylib.h)

target_include_directories(mylib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
