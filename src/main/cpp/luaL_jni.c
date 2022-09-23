//
// Created by mao on 2022/7/20.
//
#include <jni.h>
#include "lualib.h"
#include "lapi.h"
#include "lauxlib.h"

#include "utils.h"


JNIEXPORT jint JNICALL
Java_mao_commons_jlua_LuaJNI_loadBufferx0(JNIEnv *env, jclass clazz, jlong ptr, jbyteArray buffer,
                                          jstring jname, jstring jmode) {
    lua_State *l = jlong_to_ptr(ptr);
    if (buffer == NULL) {
        return 0;
    }
    jbyte *buf = (*env)->GetByteArrayElements(env, buffer, NULL);
    jsize len = (*env)->GetArrayLength(env, buffer);
    const char *name = NULL;
    if (jname != NULL) {
        name = (*env)->GetStringUTFChars(env, jname, NULL);
    }
    const char *mode = NULL;
    if (jmode != NULL) {
        mode = (*env)->GetStringUTFChars(env, jmode, NULL);
    }

    int ret = luaL_loadbufferx(l, (const char *) buf, len, name, mode);

    (*env)->ReleaseByteArrayElements(env, buffer, buf, JNI_ABORT);

    if (jname != NULL) {
        (*env)->ReleaseStringUTFChars(env, jname, name);
    }
    if (jmode != NULL) {
        (*env)->ReleaseStringUTFChars(env, jmode, mode);
    }

    return ret;
}

JNIEXPORT void JNICALL
Java_mao_commons_jlua_LuaJNI_openLibs0(JNIEnv *env, jclass clazz, jlong ptr) {
    lua_State *l = jlong_to_ptr(ptr);
    luaL_openlibs(l);
}


JNIEXPORT jlong JNICALL
Java_mao_commons_jlua_LuaJNI_checkInteger0(JNIEnv *env, jclass clazz, jlong ptr, jint arg) {
    lua_State *l = jlong_to_ptr(ptr);
    int isnum;
    lua_Integer d = lua_tointegerx(l, arg, &isnum);
    if (!isnum) {
        throw_LuaException(env, "number has no integer representation");
    }
    return d;
}

JNIEXPORT jstring JNICALL
Java_mao_commons_jlua_LuaJNI_checkLString0(JNIEnv *env, jclass clazz, jlong ptr, jint arg) {
    lua_State *l = jlong_to_ptr(ptr);

    const char *s = lua_tolstring(l, arg, NULL);
    if (!s) {
        throw_LuaException(env, "no string");
    }
    return (*env)->NewStringUTF(env, s);
}


JNIEXPORT void JNICALL
Java_mao_commons_jlua_LuaJNI_checkType0(JNIEnv *env, jclass clazz, jlong ptr, jint arg, jint t) {
    lua_State *l = jlong_to_ptr(ptr);
    if (lua_type(l, arg) != t) {
        throw_LuaException(env, lua_typename(l, t));
    }
}

JNIEXPORT jint JNICALL
Java_mao_commons_jlua_LuaJNI_ref0(JNIEnv *env, jclass clazz, jlong ptr, jint t) {
    lua_State *l = jlong_to_ptr(ptr);
    return luaL_ref(l, t);
}

JNIEXPORT void JNICALL
Java_mao_commons_jlua_LuaJNI_unref0(JNIEnv *env, jclass clazz, jlong ptr, jint t, jint ref) {
    lua_State *l = jlong_to_ptr(ptr);
    luaL_unref(l, t, ref);
}

