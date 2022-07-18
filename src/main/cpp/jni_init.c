


#include <jni.h>
#include "utils.h"

#define LOG_TAG "jlua_init"

JavaVM *javaVM;

extern void init_ids(JNIEnv *env);


jint JNI_OnLoad(JavaVM *vm, void *reserved) {
    javaVM = vm;
    JNIEnv *env;
    if ((*vm)->GetEnv(vm, (void **) &env, JNI_VERSION_1_6) != JNI_OK) {
        ALOGE("ERROR: GetEnv failed");
        return -1;
    }

    init_ids(env);


    return JNI_VERSION_1_6;
}
