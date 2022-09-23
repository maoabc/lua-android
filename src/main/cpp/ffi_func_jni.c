//
// Created by mao on 2022/7/18.
//

#include <jni.h>
#include "lua.h"
#include "lapi.h"
#include "lauxlib.h"
#include "ffi.h"
#include "utils.h"


#define CHECK_JAVA_EXCEPTION(_env, _L)                                               \
{                                                                                    \
    jthrowable occurred = (*_env)->ExceptionOccurred(_env);                          \
    if (occurred != NULL) {                                                          \
        (*_env)->ExceptionClear(_env);                                               \
        jobject str = (*_env)->CallObjectMethod(_env, occurred, getMessageMethodId); \
        if (str == NULL) {                                                           \
            str = (*_env)->CallObjectMethod(_env, occurred, toStringMethodId);       \
        }                                                                            \
        if (str != NULL) {                                                           \
            const char *utfChars = (*_env)->GetStringUTFChars(_env, str, NULL);      \
            lua_pushstring(_L, utfChars);                                            \
            (*_env)->ReleaseStringUTFChars(_env, str, utfChars);                     \
            (*_env)->DeleteLocalRef(_env, str);                                      \
        }                                                                            \
        (*_env)->DeleteLocalRef(_env, occurred);                                     \
        lua_error(_L);                                                               \
    }                                                                                \
}


static void callback(ffi_cif *cif,
                     void *ret,
                     void **args,
                     void *userdata) {
    JNIEnv *env = getJNIEnv();
    lua_State *l = *(lua_State **) args[0];
    if (env == NULL) {
        lua_pushstring(l, "No JNIEnv");
        lua_error(l);
        return;
    }

    *(int *) ret = (*env)->CallIntMethod(env, userdata, funcCallMethodId, ptr_to_jlong(l));

    //检测异常
    CHECK_JAVA_EXCEPTION(env, l);

}

typedef struct {
    ffi_closure closure;
    ffi_cif cif;
    ffi_type *args[1];
    lua_CFunction func;

} callback_func_st;

JNIEXPORT jlong JNICALL
Java_mao_commons_jlua_CJFunction_createClosure0(JNIEnv *env, jclass clazz, jobject function) {

    callback_func_st *funcSt;

    lua_CFunction func;


    funcSt = ffi_closure_alloc(sizeof(callback_func_st), (void **) &func);
    funcSt->func = func;

    funcSt->args[0] = &ffi_type_pointer;


    if (ffi_prep_cif(&funcSt->cif, FFI_DEFAULT_ABI, 1, &ffi_type_sint32, funcSt->args) != FFI_OK) {
        throw_LuaException(env, "create native closure failed");
        return 0;
    }
    jobject weakFunc = (*env)->NewWeakGlobalRef(env, function);
    if (ffi_prep_closure_loc(&funcSt->closure, &funcSt->cif, callback, weakFunc, func) != FFI_OK) {
        throw_LuaException(env, "create native closure failed");
        return 0;
    }
    return ptr_to_jlong(funcSt);
}

JNIEXPORT jlong JNICALL
Java_mao_commons_jlua_CJFunction_getCFunction0(JNIEnv *env, jclass clazz, jlong ptr) {
    callback_func_st *funcSt = jlong_to_ptr(ptr);
    return ptr_to_jlong(funcSt->func);
}

JNIEXPORT jobject JNICALL
Java_mao_commons_jlua_CJFunction_getJFunction0(JNIEnv *env, jclass clazz, jlong ptr) {
    callback_func_st *funcSt = jlong_to_ptr(ptr);
    return (jobject) funcSt->closure.user_data;
}

JNIEXPORT void JNICALL
Java_mao_commons_jlua_CJFunction_freeClosure0(JNIEnv *env, jclass clazz, jlong ptr) {
    callback_func_st *funcSt = jlong_to_ptr(ptr);
    (*env)->DeleteWeakGlobalRef(env, funcSt->closure.user_data);
    ffi_closure_free(funcSt);

}
