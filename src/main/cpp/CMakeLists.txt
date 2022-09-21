cmake_minimum_required(VERSION 3.10.2)
project(lua_jni)

add_subdirectory(lua)
add_subdirectory(mylibffi)

add_library(jlua SHARED
        lua_jni.c
        luaL_jni.c
        ffi_func_jni.c
        lua_callback_jni.c
        utils.c
        jni_init.c
        lua_test.c
        lua_android.c
        )

target_link_libraries(jlua log lua_static ffi_static)