//
// Created by mao on 2022/7/18.
//

#include <jni.h>
#include <stdlib.h>
#include "lua.h"
#include "lapi.h"
#include "lauxlib.h"
#include "ffi.h"
#include "utils.h"

static void callback(ffi_cif *cif,
                     void *ret,
                     void **args,
                     void *userdata) {
    JNIEnv *env = getJNIEnv();
    lua_State *pState = *(lua_State **) args[0];
    jlong l = ptr_to_jlong(pState);
    *(int *) ret = call_JavaFunction(env, userdata, l);
}

typedef struct {
    ffi_closure *closure;
    ffi_cif cif;
    ffi_type rtype;
    ffi_type *args[1];

} callback_func_st;

JNIEXPORT jlong JNICALL
Java_mao_commons_jlua_CJFunction_createClosure0(JNIEnv *env, jclass clazz, jobject function) {

    callback_func_st *funcSt;

    lua_CFunction func;

    funcSt = malloc(sizeof(callback_func_st));

    funcSt->closure = ffi_closure_alloc(sizeof(ffi_closure), (void **) &func);
    funcSt->args[0] = &ffi_type_pointer;

    if (ffi_prep_cif(&funcSt->cif, FFI_DEFAULT_ABI, 1, &ffi_type_sint32, funcSt->args) != FFI_OK) {
        throw_LuaException(env, "create native closure failed");
        return 0;
    }
    jobject weakFunc = (*env)->NewWeakGlobalRef(env, function);
    if (ffi_prep_closure_loc(funcSt->closure, &funcSt->cif, callback, weakFunc, func) != FFI_OK) {
        throw_LuaException(env, "create native closure failed");
        return 0;
    }
//    lua_State *l = luaL_newstate();
//    lua_pushcclosure(l, (lua_CFunction) closure, 0);
//    lua_pushinteger(l, 4);
//    lua_pushinteger(l, 6);
//    lua_call(l,2,1);
//    lua_Integer i = lua_tointeger(l, -1);
    return ptr_to_jlong(funcSt);
}

JNIEXPORT jlong JNICALL
Java_mao_commons_jlua_CJFunction_getCFunction0(JNIEnv *env, jclass clazz, jlong ptr) {
    callback_func_st *funcSt = jlong_to_ptr(ptr);
    return ptr_to_jlong(funcSt->closure);
}

JNIEXPORT jobject JNICALL
Java_mao_commons_jlua_CJFunction_getJFunction0(JNIEnv *env, jclass clazz, jlong ptr) {
    callback_func_st *funcSt = jlong_to_ptr(ptr);
    return (jobject) funcSt->closure->user_data;
}

JNIEXPORT void JNICALL
Java_mao_commons_jlua_CJFunction_freeClosure0(JNIEnv *env, jclass clazz, jlong ptr) {
    callback_func_st *funcSt = jlong_to_ptr(ptr);
    ffi_closure_free(funcSt->closure);

    free(funcSt);
}
