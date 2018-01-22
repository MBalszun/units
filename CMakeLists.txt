cmake_minimum_required(VERSION 3.5)

PROJECT(units LANGUAGES CXX)

#create library
add_library(units INTERFACE)
target_include_directories(units INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})



