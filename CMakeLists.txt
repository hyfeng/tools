cmake_minimum_required(VERSION 3.6)

project(tools)

add_definitions("-std=c++11 -Wall -g")

add_executable(choose choose.cpp)
add_executable(choose_case choose_case.cpp)
add_executable(filter_query filter_query.cpp utf8.cpp)
