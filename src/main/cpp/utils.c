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


static jclass IOExceptionClass;
static jclass LuaExceptionClass;
static jmethodID funcCall;


void throw_by_name(JNIEnv *env, const char *name, const char *msg) {
    jclass cls = (*env)->FindClass(env, name);
    if (cls != NULL) {
        (*env)->ThrowNew(env, cls, msg);
        (*env)->DeleteLocalRef(env, cls);
    }
}

void throw_IOException(JNIEnv *env, const char *msg) {
    if (msg == NULL) {
        msg = "";
    }
    (*env)->ThrowNew(env, IOExceptionClass, msg);
}

void throw_LuaException(JNIEnv *env, const char *msg) {
    if (msg == NULL) {
        msg = "";
    }
    (*env)->ThrowNew(env, LuaExceptionClass, msg);
}

int call_JavaFunction(JNIEnv *env, jobject obj, jlong luaState) {
    return (*env)->CallIntMethod(env, obj, funcCall, luaState);
}


void init_ids(JNIEnv *env) {

    IOExceptionClass = (*env)->NewGlobalRef(env, (*env)->FindClass(env, "java/io/IOException"));

    LuaExceptionClass = (*env)->NewGlobalRef(env, (*env)->FindClass(env,
                                                                    "mao/commons/jlua/LuaException"));


    jclass funcCls = (*env)->FindClass(env, "mao/commons/jlua/JFunction");
    funcCall = (*env)->GetMethodID(env, funcCls, "call", "(J)I");
    (*env)->DeleteLocalRef(env, funcCls);
}


