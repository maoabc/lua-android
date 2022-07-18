cmake_minimum_required(VERSION 3.10.2)
project(lua_jni)

add_subdirectory(lua)
add_subdirectory(mylibffi)

add_library(jlua SHARED lua_jni.c utils.c jni_init.c ffi_func_jni.c)

target_link_libraries(jlua log lua_static ffi_static)