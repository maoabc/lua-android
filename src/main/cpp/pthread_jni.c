

//
// Created by mao on 2024/1/29.
//

#include <jni.h>
#include <android/log.h>
#include <pthread.h>
#include <string.h>

#define  ALOGI(...)  __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#define  ALOGE(...)  __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)
#define  ALOGD(...)  __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__)
#define  ALOGW(...)  __android_log_print(ANDROID_LOG_WARN, LOG_TAG, __VA_ARGS__)


#define LOG_TAG "pthread"

static jmethodID runMethodId;
static jclass exceptionCls;

static JavaVM *javaVM;

static void thread_exit_handler(int sig) {
//    ALOGD("this signal is %d \n", sig);
    pthread_exit(0);
}

static void *start(void *runnable) {
    struct sigaction actions;
    memset(&actions, 0, sizeof(actions));
    sigemptyset(&actions.sa_mask);
    actions.sa_flags = 0;
    actions.sa_handler = thread_exit_handler;
    sigaction(SIGUSR2, &actions, NULL);

    JNIEnv *env;
    (*javaVM)->AttachCurrentThread(javaVM, &env, NULL);
    (*env)->CallVoidMethod(env, runnable, runMethodId);
    (*env)->DeleteWeakGlobalRef(env, runnable);
    return NULL;
}


static jlong
Java_mao_commons_thread_PThread_create0(JNIEnv *env, jclass clazz, jobject runnable) {
    pthread_t pt;
    jweak ref = (*env)->NewWeakGlobalRef(env, runnable);

    pthread_create(&pt, NULL, start, ref);


    return pt;
}

static void
Java_mao_commons_thread_PThread_cancel0(JNIEnv *env, jclass clazz, jlong tid) {
    int rc2 = pthread_kill(tid, SIGUSR2);
    if (rc2) {
        (*env)->ThrowNew(env, exceptionCls, "pthread kill failed");
        return;
    }

    pthread_join(tid, NULL);
}

#define NELEM(x) ((int) (sizeof(x) / sizeof((x)[0])))

static const JNINativeMethod methods[] = {
        {"create0", "(Ljava/lang/Runnable;)J", (void *) Java_mao_commons_thread_PThread_create0},
        {"cancel0", "(J)V",                    (void *) Java_mao_commons_thread_PThread_cancel0},

};


static jboolean registers(JNIEnv *env) {
    jclass clazz = (*env)->FindClass(env, "mao/commons/thread/PThread");
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

jint JNI_OnLoad(JavaVM *vm, void *reserved) {
    javaVM = vm;
    JNIEnv *env;
    if ((*vm)->GetEnv(vm, (void **) &env, JNI_VERSION_1_6) != JNI_OK) {
        ALOGE("ERROR: GetEnv failed");
        return -1;
    }
    jclass cls = (*env)->FindClass(env, "java/lang/Runnable");
    runMethodId = (*env)->GetMethodID(env, cls, "run", "()V");
    (*env)->DeleteLocalRef(env, cls);

    jclass expCls = (*env)->FindClass(env, "java/lang/RuntimeException");
    exceptionCls = (*env)->NewGlobalRef(env, expCls);
    (*env)->DeleteLocalRef(env, expCls);

    registers(env);

    return JNI_VERSION_1_6;
}

