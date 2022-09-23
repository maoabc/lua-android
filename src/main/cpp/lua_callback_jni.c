//
// Created by mao on 2022/7/28.
//
#include <jni.h>
#include "lua.h"
#include "lauxlib.h"
#include "utils.h"

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

JNIEXPORT int JNICALL
Java_mao_commons_jlua_LuaJavaCallbackContext_newContext0(JNIEnv *env, jclass clazz,
                                                         jlong ptr, jlongArray out
) {
    lua_State *l = jlong_to_ptr(ptr);
    //检测栈顶数据类型
    if (lua_type(l, -1) != LUA_TTABLE) {
        throw_LuaException(env, lua_typename(l, LUA_TTABLE));
        return 0;
    }
    callback_context *ctx = lua_newuserdata(l, sizeof(callback_context));
    ctx->L = lua_newthread(l);
    lua_pushcfunction(ctx->L, traceback);
    lua_setuservalue(l, -2);
    int ref = luaL_ref(l, LUA_REGISTRYINDEX);
    lua_xmove(l, ctx->L, 1);
    if (out) (*env)->SetLongArrayRegion(env, out, 0, 1, (const jlong *) &(ctx->L));
    return ref;
}

JNIEXPORT jint JNICALL
Java_mao_commons_jlua_FunctionCallbackContext_newContext0(JNIEnv *env, jclass clazz, jlong ptr,
                                                          jlongArray out) {
    lua_State *l = jlong_to_ptr(ptr);
    //检测栈顶数据类型
    if (lua_type(l, -1) != LUA_TFUNCTION) {
        throw_LuaException(env, lua_typename(l, LUA_TTABLE));
        return 0;
    }
    callback_context *ctx = lua_newuserdata(l, sizeof(callback_context));
    ctx->L = lua_newthread(l);
    lua_pushcfunction(ctx->L, traceback);
    lua_setuservalue(l, -2);
    int ref = luaL_ref(l, LUA_REGISTRYINDEX);
    lua_xmove(l, ctx->L, 1);
    if (out) (*env)->SetLongArrayRegion(env, out, 0, 1, (const jlong *) &(ctx->L));
    return ref;
}
