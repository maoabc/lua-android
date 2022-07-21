//
// Created by mao on 2022/7/20.
//
#include <jni.h>
#include "lualib.h"
#include "lapi.h"
#include "lauxlib.h"
#include "ffi.h"

#include "utils.h"

#define LOG_TAG "ffi"

static int mytest(lua_State *l) {
    lua_Integer i1 = luaL_checkinteger(l, -1);
    lua_Integer i2 = luaL_checkinteger(l, -2);
    lua_pushinteger(l, i1 + i2);
    lua_pushinteger(l, i1 * i2);
    return 2;
}


static void ttt(ffi_cif *cif,
                void *ret,
                void **args,
                void *userdata) {
//    void *l = args[0];
//    *ret = 1;
    ALOGD("ffi call ");
}

JNIEXPORT jstring JNICALL
Java_com_nmmedit_lua_NativeLib_stringFromJNI(
        JNIEnv *env, jclass clazz) {
    lua_State *l = luaL_newstate();
//    luaL_openlibs(l);

    lua_pushcclosure(l, mytest, 0);
    lua_pushinteger(l, 3);
    lua_pushinteger(l, 4);
    lua_call(l, 2, 2);
    lua_Integer i3 = lua_tointeger(l, -1);
    lua_Integer i4 = lua_tointeger(l, -2);

    lua_Integer x;
    int64_t a = 444;
    x = a;

    ffi_cif cif;
    ffi_closure *closure;

    ffi_type *args[2];
    int (*bound)(float *);

    closure = ffi_closure_alloc(sizeof(ffi_closure), (void **) &bound);
    args[0] = &ffi_type_pointer;
    args[1] = NULL;

    ffi_prep_cif(&cif, FFI_DEFAULT_ABI, 1, &ffi_type_sint32, args);
    ffi_prep_closure_loc(closure, &cif, ttt, NULL, bound);


    lua_close(l);
//
//    const char *hello = "Hello from C++";
    return NULL;
}
