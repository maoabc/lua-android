//
// Created by mao on 2023/6/24.
//

#ifndef LUA_ANDROID_LOG_H
#define LUA_ANDROID_LOG_H
#include <stdlib.h>

void android_logd(const char *str, size_t l);

/* print a string */
#define lua_writestring(s, l)   android_logd(s,l)

/* print a newline and flush the output */
#define lua_writeline()

void android_loge(const char *fmt, const char *s);
/* print an error message */
#define lua_writestringerror(s, p) \
        android_loge(s,p)

#endif //LUA_ANDROID_LOG_H
