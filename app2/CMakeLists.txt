cmake_minimum_required(VERSION 3.20)

project(App2)

add_executable(app2 prog2.cpp)

target_link_libraries(app2 PRIVATE mylib)
