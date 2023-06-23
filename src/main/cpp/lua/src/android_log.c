//
// Created by mao on 2023/6/24.
//

#include <android/log.h>
#include <string.h>
#include "android_log.h"

void android_logd(const char *str, size_t l) {
    char buf[256];
    int len;
    if (l < sizeof(buf) - 1) {
        len = l;
    } else {
        len = sizeof(buf) - 1;
    }
    memcpy(buf, str, len);
    buf[len] = 0;
    __android_log_print(ANDROID_LOG_DEBUG, "nmmLua", "%s", buf);
}


void android_loge(const char *fmt, const char *s) {
    __android_log_print(ANDROID_LOG_ERROR, "nmmLua", fmt, s);
}