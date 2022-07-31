#include <jni.h>
#include "lualib.h"
#include "lapi.h"
#include "lauxlib.h"

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

static int java_object_gc(lua_State *L) {

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
Java_mao_commons_jlua_LuaJNI_pushInteger0(JNIEnv *env, jclass clazz, jlong ptr, jlong i) {
    lua_State *l = jlong_to_ptr(ptr);
    lua_pushinteger(l, i);
}

JNIEXPORT jlong JNICALL
Java_mao_commons_jlua_LuaJNI_toInteger0(JNIEnv *env, jclass clazz, jlong ptr, jint idx) {
    lua_State *l = jlong_to_ptr(ptr);
    return lua_tointeger(l, idx);
}

JNIEXPORT jboolean JNICALL
Java_mao_commons_jlua_LuaJNI_isInteger0(JNIEnv *env, jclass clazz, jlong ptr, jint idx) {
    lua_State *l = jlong_to_ptr(ptr);
    return lua_isinteger(l, idx);
}

JNIEXPORT jboolean JNICALL
Java_mao_commons_jlua_LuaJNI_isFloat0(JNIEnv *env, jclass clazz, jlong ptr, jint idx) {
    lua_State *l = jlong_to_ptr(ptr);
    return lua_isnumber(l, idx) && !lua_isinteger(l, idx);
}

JNIEXPORT void JNICALL
Java_mao_commons_jlua_LuaJNI_pushClosure0(JNIEnv *env, jclass clazz, jlong ptr, jlong funcPtr,
                                          jint n) {
    lua_State *l = jlong_to_ptr(ptr);
    lua_pushcclosure(l, (lua_CFunction) funcPtr, n);
}

JNIEXPORT void JNICALL
Java_mao_commons_jlua_LuaJNI_pushBoolean0(JNIEnv *env, jclass clazz, jlong ptr, jboolean b) {
    lua_State *l = jlong_to_ptr(ptr);
    lua_pushboolean(l, b);
}

JNIEXPORT jboolean JNICALL
Java_mao_commons_jlua_LuaJNI_isBoolean0(JNIEnv *env, jclass clazz, jlong ptr, jint idx) {
    lua_State *l = jlong_to_ptr(ptr);
    return lua_isboolean(l, idx);
}

JNIEXPORT jboolean JNICALL
Java_mao_commons_jlua_LuaJNI_toBoolean0(JNIEnv *env, jclass clazz, jlong ptr, jint idx) {
    lua_State *l = jlong_to_ptr(ptr);
    return lua_toboolean(l, idx);
}

JNIEXPORT void JNICALL
Java_mao_commons_jlua_LuaJNI_pushNumber0(JNIEnv *env, jclass clazz, jlong ptr, jdouble num) {
    lua_State *l = jlong_to_ptr(ptr);
    lua_pushnumber(l, num);
}

JNIEXPORT jboolean JNICALL
Java_mao_commons_jlua_LuaJNI_isNumber0(JNIEnv *env, jclass clazz, jlong ptr, jint idx) {
    lua_State *l = jlong_to_ptr(ptr);
    return lua_isnumber(l, idx);
}

JNIEXPORT jdouble JNICALL
Java_mao_commons_jlua_LuaJNI_toNumber0(JNIEnv *env, jclass clazz, jlong ptr, jint idx) {
    lua_State *l = jlong_to_ptr(ptr);
    return lua_tonumber(l, idx);
}


JNIEXPORT void JNICALL
Java_mao_commons_jlua_LuaJNI_pushString0(JNIEnv *env, jclass clazz, jlong ptr, jstring str) {
    char buf[128];
    lua_State *l = jlong_to_ptr(ptr);
    if (str == NULL) {
        throw_LuaException(env, "str is null");
        return;
    }
    const jsize utf_len = (*env)->GetStringUTFLength(env, str);
    const char *string;
    if (utf_len < sizeof(buf) - 1) {//使用栈上内存
        (*env)->GetStringUTFRegion(env, str, 0, (*env)->GetStringLength(env, str), buf);
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
    lua_pushcfunction(l, java_object_gc);
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
    lua_newuserdata(l, 4);
}

JNIEXPORT int JNICALL
Java_mao_commons_jlua_LuaJNI_pcall0(JNIEnv *env, jclass clazz, jlong ptr, jint nargs, jint nresults,
                                    jint errfunc) {
    lua_State *l = jlong_to_ptr(ptr);
    return lua_pcall(l, nargs, nresults, errfunc);
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

JNIEXPORT jint JNICALL
Java_mao_commons_jlua_LuaJNI_type0(JNIEnv *env, jclass clazz, jlong ptr, jint idx) {
    lua_State *l = jlong_to_ptr(ptr);
    return lua_type(l, idx);
}

JNIEXPORT jstring JNICALL
Java_mao_commons_jlua_LuaJNI_typeName0(JNIEnv *env, jclass clazz, jlong ptr, jint type) {
    lua_State *l = jlong_to_ptr(ptr);
    const char *string = lua_typename(l, type);
    if (string == NULL) {
        return NULL;
    }
    return (*env)->NewStringUTF(env, string);
}

JNIEXPORT void JNICALL
Java_mao_commons_jlua_LuaJNI_pushNil0(JNIEnv *env, jclass clazz, jlong ptr) {
    lua_State *l = jlong_to_ptr(ptr);
    lua_pushnil(l);
}

JNIEXPORT void JNICALL
Java_mao_commons_jlua_LuaJNI_remove0(JNIEnv *env, jclass clazz, jlong ptr, jint idx) {
    lua_State *l = jlong_to_ptr(ptr);
    lua_remove(l, idx);
}

JNIEXPORT void JNICALL
Java_mao_commons_jlua_LuaJNI_createTable0(JNIEnv *env, jclass clazz, jlong ptr,
                                          jint narr, jint nrec) {
    lua_State *l = jlong_to_ptr(ptr);
    lua_createtable(l, narr, nrec);
}

JNIEXPORT jint JNICALL
Java_mao_commons_jlua_LuaJNI_getField0(JNIEnv *env, jclass clazz, jlong ptr,
                                       jint idx, jstring key) {
    lua_State *l = jlong_to_ptr(ptr);
    if (key == NULL) {
        throw_LuaException(env, "key is null");
        return 0;
    }
    const char *k = (*env)->GetStringUTFChars(env, key, NULL);

    int ret = lua_getfield(l, idx, k);

    (*env)->ReleaseStringUTFChars(env, key, k);

    return ret;
}

JNIEXPORT void JNICALL
Java_mao_commons_jlua_LuaJNI_setField0(JNIEnv *env, jclass clazz, jlong ptr,
                                       jint idx, jstring key) {
    lua_State *l = jlong_to_ptr(ptr);
    if (key == NULL) {
        throw_LuaException(env, "key is null");
        return;
    }
    const char *k = (*env)->GetStringUTFChars(env, key, NULL);

    lua_setfield(l, idx, k);

    (*env)->ReleaseStringUTFChars(env, key, k);

}

JNIEXPORT jint JNICALL
Java_mao_commons_jlua_LuaJNI_getI0(JNIEnv *env, jclass clazz, jlong ptr, jint idx, jint n) {
    lua_State *l = jlong_to_ptr(ptr);
    return lua_geti(l, idx, n);
}

JNIEXPORT void JNICALL
Java_mao_commons_jlua_LuaJNI_setI0(JNIEnv *env, jclass clazz, jlong ptr, jint idx, jint n) {
    lua_State *l = jlong_to_ptr(ptr);
    lua_seti(l, idx, n);
}

JNIEXPORT jint JNICALL
Java_mao_commons_jlua_LuaJNI_rawGetI0(JNIEnv *env, jclass clazz, jlong ptr, jint idx, jint n) {
    lua_State *l = jlong_to_ptr(ptr);
    return lua_rawgeti(l, idx, n);
}

JNIEXPORT void JNICALL
Java_mao_commons_jlua_LuaJNI_rawSetI0(JNIEnv *env, jclass clazz, jlong ptr, jint idx, jint n) {
    lua_State *l = jlong_to_ptr(ptr);
    lua_rawseti(l, idx, n);
}


JNIEXPORT void JNICALL
Java_mao_commons_jlua_LuaJNI_close0(JNIEnv *env, jclass clazz, jlong ptr) {
    lua_State *l = jlong_to_ptr(ptr);
    lua_close(l);
}

static void initIntConstant(JNIEnv *env, jclass c, const char *fieldName, int value) {
    jfieldID field = (*env)->GetStaticFieldID(env, c, fieldName, "I");
    (*env)->SetStaticIntField(env, c, field, value);
}


int register_luaJNI(JNIEnv *env) {
    jclass luaClass = (*env)->FindClass(env, "mao/commons/jlua/LuaJNI");

    initIntConstant(env, luaClass, "LUAI_MAXSTACK", LUAI_MAXSTACK);
    initIntConstant(env, luaClass, "LUA_REGISTRYINDEX", LUA_REGISTRYINDEX);

    (*env)->DeleteLocalRef(env, luaClass);

}


