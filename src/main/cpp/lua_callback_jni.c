//
// Created by mao on 2022/7/28.
//
#include <jni.h>
#include "utils.h"
#include "lua.h"
#include "lauxlib.h"

typedef struct {
    lua_State *L;
} callback_context;

static int traceback(lua_State *L) {
    const char *msg = lua_tostring(L, 1);
    if (msg)
        luaL_traceback(L, L, msg, 1);
    else {
        lua_pushliteral(L, "(no error message)");
    }
    return 1;
}

static int
Java_mao_commons_jlua_LuaCallbackContext_newContext0(JNIEnv *env, jclass clazz,
                                                     jlong ptr, jlongArray out) {
    lua_State *l = jlong_to_ptr(ptr);

    callback_context *ctx = lua_newuserdata(l, sizeof(callback_context));
    ctx->L = lua_newthread(l);
    lua_pushcfunction(ctx->L, traceback);
    lua_setuservalue(l, -2);
    int ref = luaL_ref(l, LUA_REGISTRYINDEX);
    lua_xmove(l, ctx->L, 1);
    if (out) (*env)->SetLongArrayRegion(env, out, 0, 1, (const jlong *) &(ctx->L));
    return ref;
}

#define NELEM(x) ((int) (sizeof(x) / sizeof((x)[0])))


static const JNINativeMethod methods[] = {
        {"newContext0", "(J[J)I",
         (void *) Java_mao_commons_jlua_LuaCallbackContext_newContext0},

};


jboolean register_callback_context(JNIEnv *env) {
    jclass clazz = (*env)->FindClass(env, "mao/commons/jlua/LuaCallbackContext");
    if (clazz == NULL) {
        return JNI_FALSE;
    }
    if ((*env)->RegisterNatives(env, clazz, methods, NELEM(methods)) < 0) {
        (*env)->DeleteLocalRef(env, clazz);
        return JNI_FALSE;
    }
    (*env)->DeleteLocalRef(env, clazz);

    return JNI_TRUE;
}

