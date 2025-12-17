package mao.commons.jlua;

import android.os.Build;

import androidx.annotation.NonNull;
import androidx.annotation.Nullable;

import dalvik.annotation.optimization.FastNative;

class LuaJNI {
    public static final int LUAI_MAXSTACK = 1000000;
    public static final int LUA_REGISTRYINDEX = -LUAI_MAXSTACK - 1000;


    static {
        System.loadLibrary("jlua");
    }


    //lua.h
    @FastNative
    static native long newState0();


    @FastNative
    static native void pushInteger0(long ptr, long i);

    @FastNative
    static native long toInteger0(long ptr, int idx);

    @FastNative
    static native boolean isInteger0(long ptr, int idx);

    @FastNative
    static native boolean isFloat0(long ptr, int idx);

    @FastNative
    static native void pushBoolean0(long ptr, boolean b);

    @FastNative
    static native boolean isBoolean0(long ptr, int idx);

    @FastNative
    static native boolean toBoolean0(long ptr, int idx);

    @FastNative
    static native void pushNumber0(long ptr, double num);

    @FastNative
    static native boolean isNumber0(long ptr, int idx);

    @FastNative
    static native double toNumber0(long ptr, int idx);


    @FastNative
    static native void pushClosure0(long ptr, long funcPtr, int n);

    static native void pushJavaObject0(long ptr, Object obj);

    @FastNative
    static native boolean isJavaObject0(long ptr, int idx);

    @FastNative
    static native Object toJavaObject0(long ptr, int idx);

    @FastNative
    static native Object checkJavaObject0(long ptr, int arg);

    @FastNative
    static native void pushString0(long ptr, String str);

    //lua_pushlstring
    @FastNative
    static native void pushBytes0(long ptr, byte[] bytes);

    @FastNative
    static native void pushValue0(long ptr, int idx);

    @FastNative
    static native boolean isString0(long ptr, int idx);

    @FastNative
    static native String toLString0(long ptr, int idx);

    @FastNative
    static native byte[] toRawString0(long ptr, int idx);

    @FastNative
    static native void setTop0(long ptr, int idx);

    @FastNative
    static native int getTop0(long ptr);


    @FastNative
    static native void setTable0(long ptr, int idx);

    @FastNative
    static native int getTable0(long ptr, int idx);

    @FastNative
    static native void rawset0(long ptr, int idx);

    @FastNative
    static native int rawget0(long ptr, int idx);

    @FastNative
    static native boolean rawEqual0(long ptr, int idx1, int idx2);

    @FastNative
    static native int getMetatable0(long ptr, int idx);


    static native int pcall0(long ptr, int nargs, int nresults, int errfunc);

    static native void close0(long ptr);

    @FastNative
    static native void setGlobal0(long ptr, @NonNull String global);

    @FastNative
    static native int getGlobal0(long ptr, @NonNull String global);

    @FastNative
    static native int type0(long ptr, int idx);

    @FastNative
    static native String typeName0(long ptr, int type);

    @FastNative
    static native void pushNil0(long ptr);

    @FastNative
    static native void remove0(long ptr, int idx);

    @FastNative
    static native void createTable0(long ptr, int narr, int nrec);

    @FastNative
    static native int getField0(long ptr, int idx, String key);

    @FastNative
    static native void setField0(long ptr, int idx, String key);

    @FastNative
    static native int rawGetI0(long ptr, int idx, int n);

    @FastNative
    static native void rawSetI0(long ptr, int idx, int n);

    @FastNative
    static native int getI0(long ptr, int idx, int n);

    @FastNative
    static native void setI0(long ptr, int idx, int n);

    @FastNative
    static native boolean next0(long ptr, int idx);

    @FastNative
    static native void insert0(long ptr, int idx);


    //lauxlib.h
    static native int loadBufferx0(long ptr, @NonNull byte[] buffer,
                                   @Nullable String name,
                                   @Nullable String mode);

    static native void openLibs0(long ptr);

    @FastNative
    static native long checkInteger0(long ptr, int arg);

    @FastNative
    static native double checkNumber0(long ptr, int arg);

    @FastNative
    static native String checkLString0(long ptr, int arg);

    @FastNative
    static native byte[] checkLuaBytes0(long ptr, int arg);


    @FastNative
    static native void checkType0(long ptr, int arg, int t);

    @FastNative
    static native int ref0(long ptr, int t);

    @FastNative
    static native void unref0(long ptr, int t, int ref);

//    static native void error0(long ptr, String msg);

    static native void exit0(long ptr);


}
