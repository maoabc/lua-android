//
// Created by mao on 2022/9/15.
//
#include <jni.h>
#include <stdio.h>
#include <lauxlib.h>
#include <luaconf.h>
#include "lualib.h"
#include "utils.h"

#include "lua_jni.h"

#define RESOURCE_FINDER "global_resource"
static jmethodID findResourceMethodId;

static int myLoadFile(lua_State *l, const char *fname) {
    int status = -1;
    lua_pushstring(l, RESOURCE_FINDER);
    lua_gettable(l, LUA_REGISTRYINDEX);
    jobject *finder = lua_touserdata(l, -1);
    lua_pop(l, 1);

    if (finder != NULL) {
        JNIEnv *env = getJNIEnv();
        jstring utf = (*env)->NewStringUTF(env, fname);
        //实现回调java层接口
        jbyteArray data = (*env)->CallObjectMethod(env, *finder, findResourceMethodId, utf);
        if (data != NULL) {
            jsize len = (*env)->GetArrayLength(env, data);
            jbyte *buff = (*env)->GetByteArrayElements(env, data, NULL);

            // 通过内存加载文件
            status = luaL_loadbuffer(l, (const char *) buff, len, fname);

            (*env)->ReleaseByteArrayElements(env, data, buff, JNI_ABORT);

            (*env)->DeleteLocalRef(env, data);
        }
        (*env)->DeleteLocalRef(env, utf);
    }
    return status;
}


static int luaA_custom_searcher(lua_State *L) {
    const char *fname = luaL_checklstring(L, 1, NULL);
    if (fname) {
        char filename[1024];
        snprintf(filename, sizeof(filename), "%s.lua", fname);
        if (myLoadFile(L, filename) != LUA_OK)
            return 0;
        return 1;
    }
    return 0;
}

static int luaA_setsearcher(lua_State *L) {
    lua_getglobal(L, LUA_LOADLIBNAME);
    if (lua_istable(L, -1)) {
        lua_getfield(L, -1, "searchers");
        if (lua_istable(L, -1)) {
            lua_pushcfunction(L, luaA_custom_searcher);
            lua_rawseti(L, -2, 2);
            lua_pop(L, 2);
            return 0;
        }
        lua_pop(L, 1);
    }
    lua_pop(L, 1);
    return -1;
}

static int dofilecont(lua_State *L, int d1, lua_KContext d2) {
    (void) d1;
    (void) d2;  /* only to match 'lua_Kfunction' prototype */
    return lua_gettop(L) - 1;
}


static int luaA_dofile(lua_State *L) {
    const char *fname = luaL_optstring(L, 1, NULL);
    lua_settop(L, 1);
    if (myLoadFile(L, fname) != LUA_OK)
        return 0;
    lua_callk(L, 0, LUA_MULTRET, 0, dofilecont);
    return dofilecont(L, 0, 0);
}

static int luaA_loadfile(lua_State *L) {
    const char *fname = luaL_checklstring(L, 1, NULL);
    if (fname) {
        if (myLoadFile(L, fname) != LUA_OK)
            return 0;
        return 1;
    }
    return 0;
}

JNIEXPORT void JNICALL
Java_mao_commons_jlua_LuaAndroid_setup0(JNIEnv *env, jclass clazz, jlong ptr, jobject finder) {
    lua_State *l = jlong_to_ptr(ptr);
    lua_pushstring(l, RESOURCE_FINDER);
    pushJavaObject(env, l, finder);
    lua_settable(l, LUA_REGISTRYINDEX);

    luaA_setsearcher(l);

    lua_register(l, "loadfile", luaA_loadfile);
    lua_register(l, "dofile", luaA_dofile);
}

void register_lua_android(JNIEnv *env) {
    jclass cls = (*env)->FindClass(env, "mao/commons/jlua/ResourceFinder");
    findResourceMethodId = (*env)->GetMethodID(env, cls, "findResource", "(Ljava/lang/String;)[B");
    (*env)->DeleteLocalRef(env, cls);
}