cmake_minimum_required(VERSION 3.5)

project(MbaUnits LANGUAGES CXX)

add_library(mba_units INTERFACE)
target_include_directories(mba_units INTERFACE include)

add_library(MBa::units ALIAS mba_units)


add_subdirectory(tests)
