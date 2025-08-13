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

add_definitions(
        -flto
        -O3
)
add_link_options("-Wl,--pack-dyn-relocs=android")



target_link_libraries(jlua log lua_static ffi_static)

target_link_options(jlua PUBLIC -Wl,--version-script=${CMAKE_CURRENT_SOURCE_DIR}/link.map)

#add_library(mythread SHARED pthread_jni.c)
#target_link_libraries(mythread log)
