cmake_minimum_required(VERSION 3.10.2)
project(lua)

add_subdirectory(lua)

add_library(lua SHARED lua.cpp)

target_link_libraries(lua log lua_static)