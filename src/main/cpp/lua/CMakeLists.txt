cmake_minimum_required(VERSION 3.10.2)
project(lua)

include_directories(src)

set(SRC_FILES
        src/lapi.c
        src/lcode.c
        src/lctype.c

        src/lauxlib.c
        src/lbaselib.c
        src/lcorolib.c
        src/ldblib.c
        src/liolib.c
        src/loslib.c
        src/lstrlib.c
        src/lmathlib.c
        src/ltablib.c
        src/lutf8lib.c
        src/loadlib.c

        src/ldebug.c
        src/lundump.c
        src/llex.c
        src/lparser.c
        src/ldo.c
        src/ldump.c
        src/lstate.c
        src/lgc.c
        src/linit.c
        src/lmem.c
        src/lobject.c
        src/lstring.c
        src/lfunc.c
        src/ltable.c
        src/lzio.c
        src/lvm.c
        src/ltm.c
        src/lopcodes.c

        src/android_log.c
        )

add_definitions(-DLUA_USE_C99 -Wall -Wextra )

#支持unicode变量名
add_definitions(-DLUA_UCID)

add_library(lua_static STATIC ${SRC_FILES})

target_include_directories(lua_static PUBLIC src)

target_link_libraries(lua_static)