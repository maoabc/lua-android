cmake_minimum_required(VERSION 3.10.2)
project(lua_jni)

add_subdirectory(lua)
add_subdirectory(mylibffi)

add_library(jlua SHARED
        lua_jni.c
        ffi_func_jni.c
        lua_callback_jni.c
        utils.c
        jni_init.c
        lua_android.c
        native_functions.c
        )

set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -fvisibility=hidden")


target_link_libraries(jlua log lua_static ffi_static)

#add_library(mythread SHARED pthread_jni.c)
#target_link_libraries(mythread log)
