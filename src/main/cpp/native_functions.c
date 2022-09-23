//
// Created by mao on 2022/9/22.
//
#include <jni.h>
#include "utils.h"
#include "lua.h"
#include "lauxlib.h"

static int traceback(lua_State *L) {
    const char *msg = lua_tostring(L, 1);
    if (msg)
        luaL_traceback(L, L, msg, 1);
    else {
        lua_pushliteral(L, "(no error message)");
    }
    return 1;
}


JNIEXPORT jlong JNICALL
Java_mao_commons_jlua_FunctionUtils_traceback(JNIEnv *env, jclass clazz) {
    return ptr_to_jlong(traceback);
}

