cmake_minimum_required(VERSION 3.20)

project(App1)

add_executable(app1 prog1.cpp)

target_link_libraries(app1 PRIVATE mylib)
