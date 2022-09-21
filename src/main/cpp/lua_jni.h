//
// Created by mao on 2022/9/15.
//

#ifndef LUA_JNI_H
#define LUA_JNI_H

#include <jni.h>
#include "lualib.h"

void pushJavaObject(JNIEnv *env, lua_State *l, jobject *obj);

int isJavaObject(lua_State *l, int idx);

#endif //LUA_JNI_H
