#include <jni.h>
#include "utils.h"
#include "lualib.h"
#include "lapi.h"
#include "lauxlib.h"

#include "lua_jni.h"

#define LOG_TAG "ffi"


/* Defines wheter the metatable is of a java Object */
#define LUA_JAVA_OBJECT      "__IsJavaObject"

#define LUA_TABLE_GC "__gc"


int isJavaObject(lua_State *l, int idx) {
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

//    ALOGD("luagc %p", *pObj);

    (*env)->DeleteGlobalRef(env, *pObj);


    return 0;
}


//jni

static jlong
Java_mao_commons_jlua_LuaJNI_newState0(JNIEnv *env, jclass clazz) {
    lua_State *pState = luaL_newstate();
    return ptr_to_jlong(pState);
}


static void
Java_mao_commons_jlua_LuaJNI_pushInteger0(JNIEnv *env, jclass clazz, jlong ptr, jlong i) {
    lua_State *l = jlong_to_ptr(ptr);
    lua_pushinteger(l, i);
}

static jlong
Java_mao_commons_jlua_LuaJNI_toInteger0(JNIEnv *env, jclass clazz, jlong ptr, jint idx) {
    lua_State *l = jlong_to_ptr(ptr);
    return lua_tointeger(l, idx);
}

static jboolean
Java_mao_commons_jlua_LuaJNI_isInteger0(JNIEnv *env, jclass clazz, jlong ptr, jint idx) {
    lua_State *l = jlong_to_ptr(ptr);
    return lua_isinteger(l, idx);
}

static jboolean
Java_mao_commons_jlua_LuaJNI_isFloat0(JNIEnv *env, jclass clazz, jlong ptr, jint idx) {
    lua_State *l = jlong_to_ptr(ptr);
    return lua_isnumber(l, idx) && !lua_isinteger(l, idx);
}

static void
Java_mao_commons_jlua_LuaJNI_pushClosure0(JNIEnv *env, jclass clazz, jlong ptr, jlong funcPtr,
                                          jint n) {
    lua_State *l = jlong_to_ptr(ptr);
    lua_pushcclosure(l, (lua_CFunction) funcPtr, n);
}

static void
Java_mao_commons_jlua_LuaJNI_pushBoolean0(JNIEnv *env, jclass clazz, jlong ptr, jboolean b) {
    lua_State *l = jlong_to_ptr(ptr);
    lua_pushboolean(l, b);
}

static jboolean
Java_mao_commons_jlua_LuaJNI_isBoolean0(JNIEnv *env, jclass clazz, jlong ptr, jint idx) {
    lua_State *l = jlong_to_ptr(ptr);
    return lua_isboolean(l, idx);
}

static jboolean
Java_mao_commons_jlua_LuaJNI_toBoolean0(JNIEnv *env, jclass clazz, jlong ptr, jint idx) {
    lua_State *l = jlong_to_ptr(ptr);
    return lua_toboolean(l, idx);
}

static void
Java_mao_commons_jlua_LuaJNI_pushNumber0(JNIEnv *env, jclass clazz, jlong ptr, jdouble num) {
    lua_State *l = jlong_to_ptr(ptr);
    lua_pushnumber(l, num);
}

static jboolean
Java_mao_commons_jlua_LuaJNI_isNumber0(JNIEnv *env, jclass clazz, jlong ptr, jint idx) {
    lua_State *l = jlong_to_ptr(ptr);
    return lua_isnumber(l, idx);
}

static jdouble
Java_mao_commons_jlua_LuaJNI_toNumber0(JNIEnv *env, jclass clazz, jlong ptr, jint idx) {
    lua_State *l = jlong_to_ptr(ptr);
    return lua_tonumber(l, idx);
}


static void
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
        buf[utf_len] = 0;
        string = buf;
    } else {
        string = (*env)->GetStringUTFChars(env, str, NULL);
    }

    lua_pushstring(l, string);

    if (string != buf) {
        (*env)->ReleaseStringUTFChars(env, str, string);
    }
}

static jboolean
Java_mao_commons_jlua_LuaJNI_isString0(JNIEnv *env, jclass clazz, jlong ptr, jint idx) {
    lua_State *l = jlong_to_ptr(ptr);
    return lua_isstring(l, idx);
}

static jstring
Java_mao_commons_jlua_LuaJNI_toLString0(JNIEnv *env, jclass clazz, jlong ptr, jint idx) {
    lua_State *l = jlong_to_ptr(ptr);
    const char *string = lua_tolstring(l, idx, NULL);
    if (string == NULL) {
        return NULL;
    }
    return (*env)->NewStringUTF(env, string);
}

static void
Java_mao_commons_jlua_LuaJNI_setTop0(JNIEnv *env, jclass clazz, jlong ptr, jint idx) {
    lua_State *l = jlong_to_ptr(ptr);
    lua_settop(l, idx);
}

static jint
Java_mao_commons_jlua_LuaJNI_getTop0(JNIEnv *env, jclass clazz, jlong ptr) {
    lua_State *l = jlong_to_ptr(ptr);
    return lua_gettop(l);
}


void pushJavaObject(JNIEnv *env, lua_State *l, jobject *obj) {
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
        throw_LuaException(env, "Cannot create table to java object.");
    }
}

static void
Java_mao_commons_jlua_LuaJNI_pushJavaObject0(JNIEnv *env, jclass clazz, jlong ptr, jobject obj) {
    lua_State *l = jlong_to_ptr(ptr);
    pushJavaObject(env, l, obj);
}


static jboolean
Java_mao_commons_jlua_LuaJNI_isJavaObject0(JNIEnv *env, jclass clazz, jlong ptr, jint idx) {
    lua_State *l = jlong_to_ptr(ptr);
    return isJavaObject(l, idx);
}

static jobject
Java_mao_commons_jlua_LuaJNI_toJavaObject0(JNIEnv *env, jclass clazz, jlong ptr, jint idx) {
    lua_State *l = jlong_to_ptr(ptr);
    jobject *object = lua_touserdata(l, idx);
    return *object;
}

static jobject
Java_mao_commons_jlua_LuaJNI_checkJavaObject0(JNIEnv *env, jclass clazz, jlong ptr, jint arg) {
    lua_State *l = jlong_to_ptr(ptr);
    if (!isJavaObject(l, arg)) {
        throw_LuaException(env, "Not java object");
        return 0;
    }
    jobject *object = lua_touserdata(l, arg);
    return *object;
}


static void
Java_mao_commons_jlua_LuaJNI_setTable0(JNIEnv *env, jclass clazz, jlong ptr, jint idx) {
    lua_State *l = jlong_to_ptr(ptr);
    lua_settable(l, idx);
}

static jint
Java_mao_commons_jlua_LuaJNI_getTable0(JNIEnv *env, jclass clazz, jlong ptr, jint idx) {
    lua_State *l = jlong_to_ptr(ptr);
    return lua_gettable(l, idx);
}

static void
Java_mao_commons_jlua_LuaJNI_rawset0(JNIEnv *env, jclass clazz, jlong ptr, jint idx) {
    lua_State *l = jlong_to_ptr(ptr);
    lua_rawset(l, idx);
}

static jint
Java_mao_commons_jlua_LuaJNI_rawget0(JNIEnv *env, jclass clazz, jlong ptr, jint idx) {
    lua_State *l = jlong_to_ptr(ptr);
    return lua_rawget(l, idx);
}


static jint
Java_mao_commons_jlua_LuaJNI_getMetatable0(JNIEnv *env, jclass clazz, jlong ptr, jint idx) {
    lua_State *l = jlong_to_ptr(ptr);
    return lua_getmetatable(l, idx);
}


static int
Java_mao_commons_jlua_LuaJNI_pcall0(JNIEnv *env, jclass clazz, jlong ptr, jint nargs, jint nresults,
                                    jint errfunc) {
    lua_State *l = jlong_to_ptr(ptr);
    return lua_pcall(l, nargs, nresults, errfunc);
}


static void
Java_mao_commons_jlua_LuaJNI_setGlobal0(JNIEnv *env, jclass clazz, jlong ptr, jstring global) {
    lua_State *l = jlong_to_ptr(ptr);
    if (global == NULL) {
        return;
    }
    const char *utf_global = (*env)->GetStringUTFChars(env, global, NULL);
    lua_setglobal(l, utf_global);
    (*env)->ReleaseStringUTFChars(env, global, utf_global);
}

static jint
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


static jint
Java_mao_commons_jlua_LuaJNI_type0(JNIEnv *env, jclass clazz, jlong ptr, jint idx) {
    lua_State *l = jlong_to_ptr(ptr);
    return lua_type(l, idx);
}

static jstring
Java_mao_commons_jlua_LuaJNI_typeName0(JNIEnv *env, jclass clazz, jlong ptr, jint type) {
    lua_State *l = jlong_to_ptr(ptr);
    const char *string = lua_typename(l, type);
    if (string == NULL) {
        return NULL;
    }
    return (*env)->NewStringUTF(env, string);
}

static void
Java_mao_commons_jlua_LuaJNI_pushNil0(JNIEnv *env, jclass clazz, jlong ptr) {
    lua_State *l = jlong_to_ptr(ptr);
    lua_pushnil(l);
}

static void
Java_mao_commons_jlua_LuaJNI_remove0(JNIEnv *env, jclass clazz, jlong ptr, jint idx) {
    lua_State *l = jlong_to_ptr(ptr);
    lua_remove(l, idx);
}

static void
Java_mao_commons_jlua_LuaJNI_createTable0(JNIEnv *env, jclass clazz, jlong ptr,
                                          jint narr, jint nrec) {
    lua_State *l = jlong_to_ptr(ptr);
    lua_createtable(l, narr, nrec);
}

static jint
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

static void
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

static jint
Java_mao_commons_jlua_LuaJNI_getI0(JNIEnv *env, jclass clazz, jlong ptr, jint idx, jint n) {
    lua_State *l = jlong_to_ptr(ptr);
    return lua_geti(l, idx, n);
}

static void
Java_mao_commons_jlua_LuaJNI_setI0(JNIEnv *env, jclass clazz, jlong ptr, jint idx, jint n) {
    lua_State *l = jlong_to_ptr(ptr);
    lua_seti(l, idx, n);
}


static jint
Java_mao_commons_jlua_LuaJNI_rawGetI0(JNIEnv *env, jclass clazz, jlong ptr, jint idx, jint n) {
    lua_State *l = jlong_to_ptr(ptr);
    return lua_rawgeti(l, idx, n);
}

static void
Java_mao_commons_jlua_LuaJNI_rawSetI0(JNIEnv *env, jclass clazz, jlong ptr, jint idx, jint n) {
    lua_State *l = jlong_to_ptr(ptr);
    lua_rawseti(l, idx, n);
}

static jboolean
Java_mao_commons_jlua_LuaJNI_next0(JNIEnv *env, jclass clazz, jlong ptr, jint idx) {
    lua_State *l = jlong_to_ptr(ptr);
    return lua_next(l, idx);
}

static void
Java_mao_commons_jlua_LuaJNI_insert0(JNIEnv *env, jclass clazz, jlong ptr, jint idx) {
    lua_State *l = jlong_to_ptr(ptr);
    lua_insert(l, idx);
}


static void
Java_mao_commons_jlua_LuaJNI_close0(JNIEnv *env, jclass clazz, jlong ptr) {
    lua_State *l = jlong_to_ptr(ptr);
    lua_close(l);
}


static jint
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

static void
Java_mao_commons_jlua_LuaJNI_openLibs0(JNIEnv *env, jclass clazz, jlong ptr) {
    lua_State *l = jlong_to_ptr(ptr);
    luaL_openlibs(l);
}


static jlong
Java_mao_commons_jlua_LuaJNI_checkInteger0(JNIEnv *env, jclass clazz, jlong ptr, jint arg) {
    lua_State *l = jlong_to_ptr(ptr);
    int isnum;
    lua_Integer d = lua_tointegerx(l, arg, &isnum);
    if (!isnum) {
        throw_LuaException(env, "number has no integer representation");
    }
    return d;
}

static jstring
Java_mao_commons_jlua_LuaJNI_checkLString0(JNIEnv *env, jclass clazz, jlong ptr, jint arg) {
    lua_State *l = jlong_to_ptr(ptr);

    const char *s = lua_tolstring(l, arg, NULL);
    if (!s) {
        throw_LuaException(env, "no string");
        return NULL;
    }
    return (*env)->NewStringUTF(env, s);
}


static void
Java_mao_commons_jlua_LuaJNI_checkType0(JNIEnv *env, jclass clazz, jlong ptr, jint arg, jint t) {
    lua_State *l = jlong_to_ptr(ptr);
    if (lua_type(l, arg) != t) {
        throw_LuaException(env, lua_typename(l, t));
    }
}

static jint
Java_mao_commons_jlua_LuaJNI_ref0(JNIEnv *env, jclass clazz, jlong ptr, jint t) {
    lua_State *l = jlong_to_ptr(ptr);
    return luaL_ref(l, t);
}

static void
Java_mao_commons_jlua_LuaJNI_unref0(JNIEnv *env, jclass clazz, jlong ptr, jint t, jint ref) {
    lua_State *l = jlong_to_ptr(ptr);
    luaL_unref(l, t, ref);
}

//static void
//Java_mao_commons_jlua_LuaJNI_error0(JNIEnv *env, jclass clazz, jlong ptr, jstring errorMsg) {
//    lua_State *l = jlong_to_ptr(ptr);
//    char *error = (*env)->GetStringUTFChars(env, errorMsg, NULL);
//    luaL_error(l, errorMsg);
//    (*env)->ReleaseStringUTFChars(env, errorMsg, error);
//}


#define NELEM(x) ((int) (sizeof(x) / sizeof((x)[0])))


static const JNINativeMethod methods[] = {
        {"newState0",        "()J",                                        (void *) Java_mao_commons_jlua_LuaJNI_newState0},

        {"pushInteger0",     "(JJ)V",                                      (void *) Java_mao_commons_jlua_LuaJNI_pushInteger0},

        {"toInteger0",       "(JI)J",                                      (void *) Java_mao_commons_jlua_LuaJNI_toInteger0},

        {"isInteger0",       "(JI)Z",                                      (void *) Java_mao_commons_jlua_LuaJNI_isInteger0},

        {"isFloat0",         "(JI)Z",                                      (void *) Java_mao_commons_jlua_LuaJNI_isFloat0},

        {"pushBoolean0",     "(JZ)V",                                      (void *) Java_mao_commons_jlua_LuaJNI_pushBoolean0},

        {"isBoolean0",       "(JI)Z",                                      (void *) Java_mao_commons_jlua_LuaJNI_isBoolean0},

        {"toBoolean0",       "(JI)Z",                                      (void *) Java_mao_commons_jlua_LuaJNI_toBoolean0},

        {"pushNumber0",      "(JD)V",                                      (void *) Java_mao_commons_jlua_LuaJNI_pushNumber0},

        {"isNumber0",        "(JI)Z",                                      (void *) Java_mao_commons_jlua_LuaJNI_isNumber0},

        {"toNumber0",        "(JI)D",                                      (void *) Java_mao_commons_jlua_LuaJNI_toNumber0},

        {"pushClosure0",     "(JJI)V",                                     (void *) Java_mao_commons_jlua_LuaJNI_pushClosure0},

        {"pushJavaObject0",  "(JLjava/lang/Object;)V",                     (void *) Java_mao_commons_jlua_LuaJNI_pushJavaObject0},

        {"isJavaObject0",    "(JI)Z",                                      (void *) Java_mao_commons_jlua_LuaJNI_isJavaObject0},

        {"toJavaObject0",    "(JI)Ljava/lang/Object;",                     (void *) Java_mao_commons_jlua_LuaJNI_toJavaObject0},

        {"checkJavaObject0", "(JI)Ljava/lang/Object;",                     (void *) Java_mao_commons_jlua_LuaJNI_checkJavaObject0},

        {"pushString0",      "(JLjava/lang/String;)V",                     (void *) Java_mao_commons_jlua_LuaJNI_pushString0},

        {"isString0",        "(JI)Z",                                      (void *) Java_mao_commons_jlua_LuaJNI_isString0},

        {"toLString0",       "(JI)Ljava/lang/String;",                     (void *) Java_mao_commons_jlua_LuaJNI_toLString0},

        {"setTop0",          "(JI)V",                                      (void *) Java_mao_commons_jlua_LuaJNI_setTop0},

        {"getTop0",          "(J)I",                                       (void *) Java_mao_commons_jlua_LuaJNI_getTop0},

        {"setTable0",        "(JI)V",                                      (void *) Java_mao_commons_jlua_LuaJNI_setTable0},

        {"getTable0",        "(JI)I",                                      (void *) Java_mao_commons_jlua_LuaJNI_getTable0},

        {"rawset0",          "(JI)V",                                      (void *) Java_mao_commons_jlua_LuaJNI_rawset0},

        {"rawget0",          "(JI)I",                                      (void *) Java_mao_commons_jlua_LuaJNI_rawget0},

        {"getMetatable0",    "(JI)I",                                      (void *) Java_mao_commons_jlua_LuaJNI_getMetatable0},

        {"pcall0",           "(JIII)I",                                    (void *) Java_mao_commons_jlua_LuaJNI_pcall0},

        {"close0",           "(J)V",                                       (void *) Java_mao_commons_jlua_LuaJNI_close0},

        {"setGlobal0",       "(JLjava/lang/String;)V",                     (void *) Java_mao_commons_jlua_LuaJNI_setGlobal0},

        {"getGlobal0",       "(JLjava/lang/String;)I",                     (void *) Java_mao_commons_jlua_LuaJNI_getGlobal0},

        {"type0",            "(JI)I",                                      (void *) Java_mao_commons_jlua_LuaJNI_type0},

        {"typeName0",        "(JI)Ljava/lang/String;",                     (void *) Java_mao_commons_jlua_LuaJNI_typeName0},

        {"pushNil0",         "(J)V",                                       (void *) Java_mao_commons_jlua_LuaJNI_pushNil0},

        {"remove0",          "(JI)V",                                      (void *) Java_mao_commons_jlua_LuaJNI_remove0},

        {"createTable0",     "(JII)V",                                     (void *) Java_mao_commons_jlua_LuaJNI_createTable0},

        {"getField0",        "(JILjava/lang/String;)I",                    (void *) Java_mao_commons_jlua_LuaJNI_getField0},

        {"setField0",        "(JILjava/lang/String;)V",                    (void *) Java_mao_commons_jlua_LuaJNI_setField0},

        {"rawGetI0",         "(JII)I",                                     (void *) Java_mao_commons_jlua_LuaJNI_rawGetI0},

        {"rawSetI0",         "(JII)V",                                     (void *) Java_mao_commons_jlua_LuaJNI_rawSetI0},

        {"getI0",            "(JII)I",                                     (void *) Java_mao_commons_jlua_LuaJNI_getI0},

        {"setI0",            "(JII)V",                                     (void *) Java_mao_commons_jlua_LuaJNI_setI0},

        {"next0",            "(JI)Z",                                      (void *) Java_mao_commons_jlua_LuaJNI_next0},

        {"insert0",          "(JI)V",                                      (void *) Java_mao_commons_jlua_LuaJNI_insert0},


        {"loadBufferx0",     "(J[BLjava/lang/String;Ljava/lang/String;)I", (void *) Java_mao_commons_jlua_LuaJNI_loadBufferx0},

        {"openLibs0",        "(J)V",                                       (void *) Java_mao_commons_jlua_LuaJNI_openLibs0},

        {"checkInteger0",    "(JI)J",                                      (void *) Java_mao_commons_jlua_LuaJNI_checkInteger0},

        {"checkLString0",    "(JI)Ljava/lang/String;",                     (void *) Java_mao_commons_jlua_LuaJNI_checkLString0},

        {"checkType0",       "(JII)V",                                     (void *) Java_mao_commons_jlua_LuaJNI_checkType0},

        {"ref0",             "(JI)I",                                      (void *) Java_mao_commons_jlua_LuaJNI_ref0},

        {"unref0",           "(JII)V",                                     (void *) Java_mao_commons_jlua_LuaJNI_unref0},

//        {"error0",           "(JLjava/lang/String;)V",                     (void *) Java_mao_commons_jlua_LuaJNI_error0},

};


static jboolean registerNativeMethods(JNIEnv *env) {
    jclass clazz = (*env)->FindClass(env, "mao/commons/jlua/LuaJNI");
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


static void initIntConstant(JNIEnv *env, jclass c, const char *fieldName, int value) {
    jfieldID field = (*env)->GetStaticFieldID(env, c, fieldName, "I");
    (*env)->SetStaticIntField(env, c, field, value);
}


void register_luaJNI(JNIEnv *env) {
    jclass luaClass = (*env)->FindClass(env, "mao/commons/jlua/LuaJNI");

    initIntConstant(env, luaClass, "LUAI_MAXSTACK", LUAI_MAXSTACK);
    initIntConstant(env, luaClass, "LUA_REGISTRYINDEX", LUA_REGISTRYINDEX);

    (*env)->DeleteLocalRef(env, luaClass);

    registerNativeMethods(env);
}



