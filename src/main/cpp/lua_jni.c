#include <jni.h>
#include "lualib.h"
#include "lapi.h"
#include "lauxlib.h"
#include "ffi.h"

#include "utils.h"

#define LOG_TAG "ffi"

static int mytest(lua_State *l) {
    lua_Integer i1 = luaL_checkinteger(l, -1);
    lua_Integer i2 = luaL_checkinteger(l, -2);
    lua_pushinteger(l, i1 + i2);
    lua_pushinteger(l, i1 * i2);
    return 2;
}

static int cfunc_to_jfunc(lua_State *l) {
    JNIEnv *env = getJNIEnv();
    jobject obj = lua_touserdata(l, 1);
    return call_JavaFunction(env, obj, ptr_to_jlong(l));
}

static void ttt(ffi_cif *cif,
                void *ret,
                void **args,
                void *userdata) {
//    void *l = args[0];
//    *ret = 1;
    ALOGD("ffi call ");
}


JNIEXPORT jstring JNICALL
Java_com_nmmedit_lua_NativeLib_stringFromJNI(
        JNIEnv *env, jclass clazz) {
    lua_State *l = luaL_newstate();
//    luaL_openlibs(l);

    lua_pushcclosure(l, mytest, 0);
    lua_pushinteger(l, 3);
    lua_pushinteger(l, 4);
    lua_call(l, 2, 2);
    lua_Integer i3 = lua_tointeger(l, -1);
    lua_Integer i4 = lua_tointeger(l, -2);

    lua_Integer x;
    int64_t a = 444;
    x = a;

    ffi_cif cif;
    ffi_closure *closure;

    ffi_type *args[2];
    int (*bound)(float *);

    closure = ffi_closure_alloc(sizeof(ffi_closure), &bound);
    args[0] = &ffi_type_pointer;
    args[1] = NULL;

    ffi_prep_cif(&cif, FFI_DEFAULT_ABI, 1, &ffi_type_sint32, args);
    ffi_prep_closure_loc(closure, &cif, ttt, NULL, bound);


    lua_close(l);
//
//    const char *hello = "Hello from C++";
    return NULL;
}

JNIEXPORT jlong JNICALL
Java_mao_commons_jlua_LuaState_newState0(JNIEnv *env, jclass clazz) {
    lua_State *pState = luaL_newstate();
    jlong i = ptr_to_jlong(pState);
    return i;
}

JNIEXPORT jint JNICALL
Java_mao_commons_jlua_LuaState_checkInteger0(JNIEnv *env, jclass clazz, jlong ptr, jint arg) {
    lua_State *l = jlong_to_ptr(ptr);
    return (jint) luaL_checkinteger(l, arg);
}

JNIEXPORT void JNICALL
Java_mao_commons_jlua_LuaState_pushInteger0(JNIEnv *env, jclass clazz, jlong ptr, jint i) {
    lua_State *l = jlong_to_ptr(ptr);
    lua_pushinteger(l, i);
}

JNIEXPORT jint JNICALL
Java_mao_commons_jlua_LuaState_toInteger0(JNIEnv *env, jclass clazz, jlong ptr, jint idx) {
    lua_State *l = jlong_to_ptr(ptr);
    return (jint) lua_tointeger(l, idx);
}

JNIEXPORT void JNICALL
Java_mao_commons_jlua_LuaState_pushClosure0(JNIEnv *env, jclass clazz, jlong ptr, jlong funcPtr,
                                            jint n) {
    lua_State *l = jlong_to_ptr(ptr);
    lua_pushcclosure(l, (lua_CFunction) funcPtr, n);
}


JNIEXPORT void JNICALL
Java_mao_commons_jlua_LuaState_call0(JNIEnv *env, jclass clazz, jlong ptr, jint nargs,
                                     jint nresults) {
    lua_State *l = jlong_to_ptr(ptr);
    lua_call(l, nargs, nresults);
}


JNIEXPORT void JNICALL
Java_mao_commons_jlua_LuaState_close0(JNIEnv *env, jclass clazz, jlong ptr) {
    lua_State *l = jlong_to_ptr(ptr);
    lua_close(l);
}



