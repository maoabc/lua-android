//
// Created by mao on 2022/7/20.
//
#include <jni.h>
#include "lualib.h"
#include "lapi.h"
#include "lauxlib.h"

#include "utils.h"


JNIEXPORT jint JNICALL
Java_mao_commons_jlua_LuaJNI_loadBufferx0(JNIEnv *env, jclass clazz, jlong ptr, jstring buffer,
                                          jstring jname, jstring jmode) {
    lua_State *l = jlong_to_ptr(ptr);
    if (buffer == NULL) {
        return 0;
    }
    const char *utf_buf = (*env)->GetStringUTFChars(env, buffer, NULL);
    jsize utf_len = (*env)->GetStringUTFLength(env, buffer);
    const char *name = NULL;
    if (jname != NULL) {
        name = (*env)->GetStringUTFChars(env, jname, NULL);
    }
    const char *mode = NULL;
    if (jmode != NULL) {
        mode = (*env)->GetStringUTFChars(env, jmode, NULL);
    }

    int ret = luaL_loadbufferx(l, utf_buf, utf_len, name, mode);

    (*env)->ReleaseStringUTFChars(env, buffer, utf_buf);
    if (jname != NULL) {
        (*env)->ReleaseStringUTFChars(env, jname, name);
    }
    if (jmode != NULL) {
        (*env)->ReleaseStringUTFChars(env, jmode, mode);
    }

    return ret;
}


JNIEXPORT jint JNICALL
Java_mao_commons_jlua_LuaJNI_checkInteger0(JNIEnv *env, jclass clazz, jlong ptr, jint arg) {
    lua_State *l = jlong_to_ptr(ptr);
    return (jint) luaL_checkinteger(l, arg);
}

JNIEXPORT jstring JNICALL
Java_mao_commons_jlua_LuaJNI_checkLString0(JNIEnv *env, jclass clazz, jlong ptr, jint arg) {
    lua_State *l = jlong_to_ptr(ptr);

    const char *string = luaL_checklstring(l, arg, NULL);
    if (string == NULL) {
        return NULL;
    }
    return (*env)->NewStringUTF(env, string);
}

