//
// Created by mao on 21-2-18.
//
#include <stdlib.h>
#include "utils.h"

#define LOG_TAG "jlua"

extern JavaVM *javaVM;

JNIEnv *getJNIEnv() {
    JNIEnv *env;
    if ((*javaVM)->GetEnv(javaVM, (void **) &env, JNI_VERSION_1_6) != JNI_OK) {
        ALOGE("error GetEnv");
        return NULL;
    }

    //目前应该不用考虑在native层开线程
//    (*javaVM)->AttachCurrentThread(javaVM, &env, NULL);

    return env;
}


static jclass LuaExceptionClass;

jmethodID funcCallMethodId;
jmethodID toStringMethodId;
jmethodID getMessageMethodId;


void throw_by_name(JNIEnv *env, const char *name, const char *msg) {
    jclass cls = (*env)->FindClass(env, name);
    if (cls != NULL) {
        (*env)->ThrowNew(env, cls, msg);
        (*env)->DeleteLocalRef(env, cls);
    }
}

void throw_LuaException(JNIEnv *env, const char *msg) {
    if (msg == NULL) {
        msg = "";
    }
    (*env)->ThrowNew(env, LuaExceptionClass, msg);
}


void init_ids(JNIEnv *env) {


    LuaExceptionClass = (*env)->NewGlobalRef(env, (*env)->FindClass(env,
                                                                    "mao/commons/jlua/LuaException"));


    jclass funcCls = (*env)->FindClass(env, "mao/commons/jlua/CJFunction");
    funcCallMethodId = (*env)->GetMethodID(env, funcCls, "call", "(J)I");
    (*env)->DeleteLocalRef(env, funcCls);

    jclass throwableCls = (*env)->FindClass(env, "java/lang/Throwable");
    toStringMethodId = (*env)->GetMethodID(env, throwableCls, "toString", "()Ljava/lang/String;");
    getMessageMethodId = (*env)->GetMethodID(env, throwableCls, "getMessage",
                                             "()Ljava/lang/String;");
    (*env)->DeleteLocalRef(env, throwableCls);
}


