#include <jni.h>
#include "lualib.h"
#include "lapi.h"
#include "lauxlib.h"
#include "ffi.h"

#include "utils.h"

#define LOG_TAG "ffi"


/* Defines wheter the metatable is of a java Object */
#define LUA_JAVA_OBJECT      "__IsJavaObject"

#define LUA_TABLE_GC "__gc"


static int isJavaObject(lua_State *l, int idx) {
    if (!lua_isuserdata(l, idx))
        return 0;

    if (lua_getmetatable(l, idx) == 0)
        return 0;

    lua_pushstring(l, LUA_JAVA_OBJECT);
    lua_rawget(l, -2);

    if (lua_isnil(l, -1)) {
        lua_pop(l, 2);
        return 0;
    }
    lua_pop(l, 2);
    return 1;
}

static int gc(lua_State *L) {

    if (!isJavaObject(L, 1)) {
        return 0;
    }
    JNIEnv *env = getJNIEnv();
    /* Gets the JNI Environment */
    if (env == NULL) {
        lua_pushstring(L, "Invalid JNI Environment.");
        lua_error(L);
    }

    jobject *pObj = (jobject *) lua_touserdata(L, 1);

    ALOGD("luagc %p", *pObj);

    (*env)->DeleteGlobalRef(env, *pObj);


    return 0;
}


//jni

JNIEXPORT jlong JNICALL
Java_mao_commons_jlua_LuaJNI_newState0(JNIEnv *env, jclass clazz) {
    lua_State *pState = luaL_newstate();
    jlong i = ptr_to_jlong(pState);
    return i;
}


JNIEXPORT void JNICALL
Java_mao_commons_jlua_LuaJNI_pushInteger0(JNIEnv *env, jclass clazz, jlong ptr, jint i) {
    lua_State *l = jlong_to_ptr(ptr);
    lua_pushinteger(l, i);
}

JNIEXPORT jint JNICALL
Java_mao_commons_jlua_LuaJNI_toInteger0(JNIEnv *env, jclass clazz, jlong ptr, jint idx) {
    lua_State *l = jlong_to_ptr(ptr);
    return (jint) lua_tointeger(l, idx);
}

JNIEXPORT void JNICALL
Java_mao_commons_jlua_LuaJNI_pushClosure0(JNIEnv *env, jclass clazz, jlong ptr, jlong funcPtr,
                                          jint n) {
    lua_State *l = jlong_to_ptr(ptr);
    lua_pushcclosure(l, (lua_CFunction) funcPtr, n);
}


JNIEXPORT void JNICALL
Java_mao_commons_jlua_LuaJNI_pushString0(JNIEnv *env, jclass clazz, jlong ptr, jstring str) {
    char buf[128];
    lua_State *l = jlong_to_ptr(ptr);
    if (str == NULL) {
        return;
    }
    const jsize utf_len = (*env)->GetStringUTFLength(env, str);
    const char *string;
    if (utf_len < sizeof(buf) - 1) {//使用栈上内存
        jsize strLen = (*env)->GetStringLength(env, str);
        (*env)->GetStringUTFRegion(env, str, 0, strLen, buf);
        string = buf;
    } else {
        string = (*env)->GetStringUTFChars(env, str, NULL);
    }

    lua_pushlstring(l, string, utf_len);

    if (string != buf) {
        (*env)->ReleaseStringUTFChars(env, str, string);
    }
}

JNIEXPORT jboolean JNICALL
Java_mao_commons_jlua_LuaJNI_isString0(JNIEnv *env, jclass clazz, jlong ptr, jint idx) {
    lua_State *l = jlong_to_ptr(ptr);
    return lua_isstring(l, idx);
}

JNIEXPORT jstring JNICALL
Java_mao_commons_jlua_LuaJNI_toLString0(JNIEnv *env, jclass clazz, jlong ptr, jint idx) {
    lua_State *l = jlong_to_ptr(ptr);
    const char *string = lua_tolstring(l, idx, NULL);
    if (string == NULL) {
        return NULL;
    }
    return (*env)->NewStringUTF(env, string);
}

JNIEXPORT void JNICALL
Java_mao_commons_jlua_LuaJNI_setTop0(JNIEnv *env, jclass clazz, jlong ptr, jint idx) {
    lua_State *l = jlong_to_ptr(ptr);
    lua_settop(l, idx);
}

JNIEXPORT jint JNICALL
Java_mao_commons_jlua_LuaJNI_getTop0(JNIEnv *env, jclass clazz, jlong ptr) {
    lua_State *l = jlong_to_ptr(ptr);
    return lua_gettop(l);
}


JNIEXPORT void JNICALL
Java_mao_commons_jlua_LuaJNI_pushJavaObject0(JNIEnv *env, jclass clazz, jlong ptr, jobject obj) {
    lua_State *l = jlong_to_ptr(ptr);
    jobject *userdata = lua_newuserdata(l, sizeof(jobject));
    *userdata = (*env)->NewGlobalRef(env, obj);
    lua_newtable(l);

    lua_pushstring(l, LUA_TABLE_GC);
    lua_pushcfunction(l, gc);
    lua_rawset(l, -3);
    /* Is Java Object boolean */
    lua_pushstring(l, LUA_JAVA_OBJECT);
    lua_pushboolean(l, 1);
    lua_rawset(l, -3);

    if (lua_setmetatable(l, -2) == 0) {
        lua_pushstring(l, "Cannot create table to java object.");
        lua_error(l);
    }

}


JNIEXPORT jboolean JNICALL
Java_mao_commons_jlua_LuaJNI_isJavaObject0(JNIEnv *env, jclass clazz, jlong ptr, jint idx) {
    lua_State *l = jlong_to_ptr(ptr);
    return isJavaObject(l, idx);
}

JNIEXPORT jobject JNICALL
Java_mao_commons_jlua_LuaJNI_toJavaObject0(JNIEnv *env, jclass clazz, jlong ptr, jint idx) {
    lua_State *l = jlong_to_ptr(ptr);
    jobject *object = lua_touserdata(l, idx);
    return *object;
}

JNIEXPORT jobject JNICALL
Java_mao_commons_jlua_LuaJNI_checkJavaObject0(JNIEnv *env, jclass clazz, jlong ptr, jint arg) {
    lua_State *l = jlong_to_ptr(ptr);
    if (!isJavaObject(l, arg)) {
        luaL_argerror(l, arg, "Not java object");
        return 0;
    }
    jobject *object = lua_touserdata(l, arg);
    return *object;
}


JNIEXPORT void JNICALL
Java_mao_commons_jlua_LuaJNI_setTable0(JNIEnv *env, jclass clazz, jlong ptr, jint idx) {
    lua_State *l = jlong_to_ptr(ptr);
    lua_settable(l, idx);
}

JNIEXPORT jint JNICALL
Java_mao_commons_jlua_LuaJNI_getTable0(JNIEnv *env, jclass clazz, jlong ptr, jint idx) {
    lua_State *l = jlong_to_ptr(ptr);
    return lua_gettable(l, idx);
}

JNIEXPORT void JNICALL
Java_mao_commons_jlua_LuaJNI_rawset0(JNIEnv *env, jclass clazz, jlong ptr, jint idx) {
    lua_State *l = jlong_to_ptr(ptr);
    lua_rawset(l, idx);
}

JNIEXPORT jint JNICALL
Java_mao_commons_jlua_LuaJNI_rawget0(JNIEnv *env, jclass clazz, jlong ptr, jint idx) {
    lua_State *l = jlong_to_ptr(ptr);
    return lua_rawget(l, idx);
}


JNIEXPORT jint JNICALL
Java_mao_commons_jlua_LuaJNI_getMetatable0(JNIEnv *env, jclass clazz, jlong ptr, jint idx) {
    lua_State *l = jlong_to_ptr(ptr);
    return lua_getmetatable(l, idx);
}


JNIEXPORT void JNICALL
Java_mao_commons_jlua_LuaJNI_call0(JNIEnv *env, jclass clazz, jlong ptr, jint nargs,
                                   jint nresults) {
    lua_State *l = jlong_to_ptr(ptr);
    lua_call(l, nargs, nresults);
}

JNIEXPORT void JNICALL
Java_mao_commons_jlua_LuaJNI_pcall0(JNIEnv *env, jclass clazz, jlong ptr, jint nargs, jint nresults,
                                    jint errfunc) {
    lua_State *l = jlong_to_ptr(ptr);
    lua_pcall(l, nargs, nresults, errfunc);
}


JNIEXPORT void JNICALL
Java_mao_commons_jlua_LuaJNI_setGlobal0(JNIEnv *env, jclass clazz, jlong ptr, jstring global) {
    lua_State *l = jlong_to_ptr(ptr);
    if (global == NULL) {
        return;
    }
    const char *utf_global = (*env)->GetStringUTFChars(env, global, NULL);
    lua_setglobal(l, utf_global);
    (*env)->ReleaseStringUTFChars(env, global, utf_global);
}

JNIEXPORT jint JNICALL
Java_mao_commons_jlua_LuaJNI_getGlobal0(JNIEnv *env, jclass clazz, jlong ptr, jstring global) {
    lua_State *l = jlong_to_ptr(ptr);
    if (global == NULL) {
        return 0;
    }
    const char *utf_global = (*env)->GetStringUTFChars(env, global, NULL);
    int ret = lua_getglobal(l, utf_global);
    (*env)->ReleaseStringUTFChars(env, global, utf_global);
    return ret;
}

JNIEXPORT jint JNICALL
Java_mao_commons_jlua_LuaJNI_error0(JNIEnv *env, jclass clazz, jlong ptr) {
    lua_State *l = jlong_to_ptr(ptr);
    return lua_error(l);
}


JNIEXPORT void JNICALL
Java_mao_commons_jlua_LuaJNI_close0(JNIEnv *env, jclass clazz, jlong ptr) {
    lua_State *l = jlong_to_ptr(ptr);
    lua_close(l);
}



