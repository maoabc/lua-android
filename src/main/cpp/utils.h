//
// Created by mao on 20-4-16.
//

#ifndef LUA_JNI_UTIL_H
#define LUA_JNI_UTIL_H

#include <jni.h>
#include <android/log.h>

#ifdef __cplusplus
extern "C" {
#endif


#ifdef _LP64
#define jlong_to_ptr(a) ((void*)(a))
#define ptr_to_jlong(a) ((jlong)(a))
#else
#define jlong_to_ptr(a) ((void*)(int)(a))
#define ptr_to_jlong(a) ((jlong)(int)(a))
#endif

JNIEnv *getJNIEnv();

void throw_by_name(JNIEnv *env, const char *name, const char *msg);

void throw_IOException(JNIEnv *env, const char *msg);

void throw_LuaException(JNIEnv *env, const char *msg);



#define  ALOGI(...)  __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#define  ALOGE(...)  __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)
#define  ALOGD(...)  __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__)
#define  ALOGW(...)  __android_log_print(ANDROID_LOG_WARN, LOG_TAG, __VA_ARGS__)

jmethodID funcCallMethodId;
jmethodID toStringMethodId;

#ifdef __cplusplus
}
#endif

#endif //LUA_JNI_UTIL_H
