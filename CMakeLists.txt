cmake_minimum_required(VERSION 3.5)

project(units LANGUAGES CXX)

add_library(units INTERFACE)
target_include_directories(units INTERFACE include)

add_library(MBa::units ALIAS units)
