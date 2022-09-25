


#include <jni.h>
#include "utils.h"

#define LOG_TAG "jlua_init"

JavaVM *javaVM;

extern void init_ids(JNIEnv *env);

extern void register_luaJNI(JNIEnv *env);

extern void register_lua_android(JNIEnv *env);

extern jboolean register_callback_context(JNIEnv *env);

extern jboolean register_ffi_function(JNIEnv *env);

extern jboolean register_util_functions(JNIEnv *env);

jint JNI_OnLoad(JavaVM *vm, void *reserved) {
    javaVM = vm;
    JNIEnv *env;
    if ((*vm)->GetEnv(vm, (void **) &env, JNI_VERSION_1_6) != JNI_OK) {
        ALOGE("ERROR: GetEnv failed");
        return -1;
    }

    init_ids(env);

    register_luaJNI(env);

    register_lua_android(env);

    register_callback_context(env);

    register_ffi_function(env);

    register_util_functions(env);


    return JNI_VERSION_1_6;
}
