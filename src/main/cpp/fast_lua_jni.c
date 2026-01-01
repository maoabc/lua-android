#include <jni.h>
#include "utils.h"
#include "lualib.h"
#include "lapi.h"
#include "lauxlib.h"



//jni


static void
Java_mao_commons_jlua_FastLuaJNI_pushInteger0(jlong ptr, jlong i) {
    lua_State *l = jlong_to_ptr(ptr);
    lua_pushinteger(l, i);
}

static jlong
Java_mao_commons_jlua_FastLuaJNI_toInteger0(jlong ptr, jint idx) {
    lua_State *l = jlong_to_ptr(ptr);
    return lua_tointeger(l, idx);
}

static jboolean
Java_mao_commons_jlua_FastLuaJNI_isInteger0(jlong ptr, jint idx) {
    lua_State *l = jlong_to_ptr(ptr);
    return lua_isinteger(l, idx);
}

static jboolean
Java_mao_commons_jlua_FastLuaJNI_isFloat0(jlong ptr, jint idx) {
    lua_State *l = jlong_to_ptr(ptr);
    return lua_isnumber(l, idx) && !lua_isinteger(l, idx);
}

static void
Java_mao_commons_jlua_FastLuaJNI_pushClosure0(jlong ptr, jlong funcPtr,
                                              jint n) {
    lua_State *l = jlong_to_ptr(ptr);
    lua_pushcclosure(l, (lua_CFunction) funcPtr, n);
}

static void
Java_mao_commons_jlua_FastLuaJNI_pushBoolean0(jlong ptr, jboolean b) {
    lua_State *l = jlong_to_ptr(ptr);
    lua_pushboolean(l, b);
}

static jboolean
Java_mao_commons_jlua_FastLuaJNI_isBoolean0(jlong ptr, jint idx) {
    lua_State *l = jlong_to_ptr(ptr);
    return lua_isboolean(l, idx);
}

static jboolean
Java_mao_commons_jlua_FastLuaJNI_toBoolean0(jlong ptr, jint idx) {
    lua_State *l = jlong_to_ptr(ptr);
    return lua_toboolean(l, idx);
}

static void
Java_mao_commons_jlua_FastLuaJNI_pushNumber0(jlong ptr, jdouble num) {
    lua_State *l = jlong_to_ptr(ptr);
    lua_pushnumber(l, num);
}

static jboolean
Java_mao_commons_jlua_FastLuaJNI_isNumber0(jlong ptr, jint idx) {
    lua_State *l = jlong_to_ptr(ptr);
    return lua_isnumber(l, idx);
}

static jdouble
Java_mao_commons_jlua_FastLuaJNI_toNumber0(jlong ptr, jint idx) {
    lua_State *l = jlong_to_ptr(ptr);
    return lua_tonumber(l, idx);
}


static void
Java_mao_commons_jlua_FastLuaJNI_pushValue0(jlong ptr, jint idx) {
    lua_State *l = jlong_to_ptr(ptr);
    lua_pushvalue(l, idx);
}

static jboolean
Java_mao_commons_jlua_FastLuaJNI_isString0(jlong ptr, jint idx) {
    lua_State *l = jlong_to_ptr(ptr);
    return lua_isstring(l, idx);
}

static void
Java_mao_commons_jlua_FastLuaJNI_setTop0(jlong ptr, jint idx) {
    lua_State *l = jlong_to_ptr(ptr);
    lua_settop(l, idx);
}

static jint
Java_mao_commons_jlua_FastLuaJNI_getTop0(jlong ptr) {
    lua_State *l = jlong_to_ptr(ptr);
    return lua_gettop(l);
}


static jint
Java_mao_commons_jlua_FastLuaJNI_type0(jlong ptr, jint idx) {
    lua_State *l = jlong_to_ptr(ptr);
    return lua_type(l, idx);
}


static void
Java_mao_commons_jlua_FastLuaJNI_pushNil0(jlong ptr) {
    lua_State *l = jlong_to_ptr(ptr);
    lua_pushnil(l);
}

static void
Java_mao_commons_jlua_FastLuaJNI_remove0(jlong ptr, jint idx) {
    lua_State *l = jlong_to_ptr(ptr);
    lua_remove(l, idx);
}


static void
Java_mao_commons_jlua_FastLuaJNI_rotate0(jlong ptr, jint idx, int n) {
    lua_State *l = jlong_to_ptr(ptr);
    lua_rotate(l, idx, n);
}

static jlong
Java_mao_commons_jlua_FastLuaJNI_findRootThread0(jlong ptr) {
    lua_State *current = jlong_to_ptr(ptr);
    while (1) {
        lua_rawgeti(current, LUA_REGISTRYINDEX, LUA_RIDX_MAINTHREAD);
        lua_State *main = lua_tothread(current, -1);
        lua_pop(current, 1);

        if (main == NULL || main == current) {
            return ptr_to_jlong(current);  // 已经是最顶层
        }
        current = main;
    }
}


#define NELEM(x) ((int) (sizeof(x) / sizeof((x)[0])))


static const JNINativeMethod methods[] = {

        {"pushInteger0",    "(JJ)V",  (void *) Java_mao_commons_jlua_FastLuaJNI_pushInteger0},

        {"toInteger0",      "(JI)J",  (void *) Java_mao_commons_jlua_FastLuaJNI_toInteger0},

        {"isInteger0",      "(JI)Z",  (void *) Java_mao_commons_jlua_FastLuaJNI_isInteger0},

        {"isFloat0",        "(JI)Z",  (void *) Java_mao_commons_jlua_FastLuaJNI_isFloat0},

        {"pushBoolean0",    "(JZ)V",  (void *) Java_mao_commons_jlua_FastLuaJNI_pushBoolean0},

        {"isBoolean0",      "(JI)Z",  (void *) Java_mao_commons_jlua_FastLuaJNI_isBoolean0},

        {"toBoolean0",      "(JI)Z",  (void *) Java_mao_commons_jlua_FastLuaJNI_toBoolean0},

        {"pushNumber0",     "(JD)V",  (void *) Java_mao_commons_jlua_FastLuaJNI_pushNumber0},

        {"isNumber0",       "(JI)Z",  (void *) Java_mao_commons_jlua_FastLuaJNI_isNumber0},

        {"toNumber0",       "(JI)D",  (void *) Java_mao_commons_jlua_FastLuaJNI_toNumber0},

        {"pushClosure0",    "(JJI)V", (void *) Java_mao_commons_jlua_FastLuaJNI_pushClosure0},

        {"pushValue0",      "(JI)V",  (void *) Java_mao_commons_jlua_FastLuaJNI_pushValue0},

        {"isString0",       "(JI)Z",  (void *) Java_mao_commons_jlua_FastLuaJNI_isString0},

        {"setTop0",         "(JI)V",  (void *) Java_mao_commons_jlua_FastLuaJNI_setTop0},

        {"getTop0",         "(J)I",   (void *) Java_mao_commons_jlua_FastLuaJNI_getTop0},

        {"type0",           "(JI)I",  (void *) Java_mao_commons_jlua_FastLuaJNI_type0},

        {"pushNil0",        "(J)V",   (void *) Java_mao_commons_jlua_FastLuaJNI_pushNil0},

        {"remove0",         "(JI)V",  (void *) Java_mao_commons_jlua_FastLuaJNI_remove0},

        {"rotate0",         "(JII)V", (void *) Java_mao_commons_jlua_FastLuaJNI_rotate0},

        {"findRootThread0", "(J)J",   (void *) Java_mao_commons_jlua_FastLuaJNI_findRootThread0},
};


static jboolean registerNativeMethods(JNIEnv *env) {
    jclass clazz = (*env)->FindClass(env, "mao/commons/jlua/FastLuaJNI");
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


void register_fast_luaJNI(JNIEnv *env) {

    registerNativeMethods(env);
}



