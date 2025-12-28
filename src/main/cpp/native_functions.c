//
// Created by mao on 2022/9/22.
//
#include <jni.h>
#include "utils.h"
#include "lua.h"
#include "lauxlib.h"

static int traceback(lua_State *L) {
    const char *msg = lua_tostring(L, 1);
    if (msg) {
        luaL_traceback(L, L, msg, 1);
    } else {
        lua_pushliteral(L, "(no error message)");
    }
    return 1;
}

static int empty_func(lua_State *L) {
    return 0;
}

static jlong
Java_mao_commons_jlua_UtilFunctions_traceback(JNIEnv *env, jclass clazz) {
    return ptr_to_jlong(traceback);
}

static jlong
Java_mao_commons_jlua_UtilFunctions_emptyFunc(JNIEnv *env, jclass clazz) {
    return ptr_to_jlong(empty_func);
}


#define NELEM(x) ((int) (sizeof(x) / sizeof((x)[0])))

static const JNINativeMethod methods[] = {
        {"traceback", "()J", (void *) Java_mao_commons_jlua_UtilFunctions_traceback},

        {"emptyFunc", "()J", (void *) Java_mao_commons_jlua_UtilFunctions_emptyFunc},

};


jboolean register_util_functions(JNIEnv *env) {
    jclass clazz = (*env)->FindClass(env, "mao/commons/jlua/UtilFunctions");
    if (clazz == NULL) {
        return JNI_FALSE;
    }
    if ((*env)->RegisterNatives(env, clazz, methods, NELEM(methods)) < 0) {
        return JNI_FALSE;
    }

    return JNI_TRUE;
}

