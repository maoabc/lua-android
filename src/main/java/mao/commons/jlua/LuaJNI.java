package mao.commons.jlua;

import androidx.annotation.NonNull;
import androidx.annotation.Nullable;

class LuaJNI {
    public static final int LUAI_MAXSTACK = placeholder();
    public static final int LUA_REGISTRYINDEX = placeholder();



    static {
        System.loadLibrary("jlua");
    }

    // A hack to avoid these constants being inlined by javac...
    private static int placeholder() {
        return 0;
    }


    //lua.h
    static native long newState0();


    static native void pushInteger0(long ptr, long i);

    static native long toInteger0(long ptr, int idx);

    static native boolean isInteger0(long ptr, int idx);

    static native boolean isFloat0(long ptr, int idx);

    static native void pushBoolean0(long ptr, boolean b);

    static native boolean isBoolean0(long ptr, int idx);

    static native boolean toBoolean0(long ptr, int idx);

    static native void pushNumber0(long ptr, double num);

    static native boolean isNumber0(long ptr, int idx);

    static native double toNumber0(long ptr, int idx);


    static native void pushClosure0(long ptr, long funcPtr, int n);

    static native void pushJavaObject0(long ptr, Object obj);

    static native boolean isJavaObject0(long ptr, int idx);

    static native Object toJavaObject0(long ptr, int idx);

    static native Object checkJavaObject0(long ptr, int arg);

    static native void pushString0(long ptr, String str);

    static native boolean isString0(long ptr, int idx);

    static native String toLString0(long ptr, int idx);

    static native void setTop0(long ptr, int idx);

    static native int getTop0(long ptr);


    static native void setTable0(long ptr, int idx);

    static native int getTable0(long ptr, int idx);

    static native void rawset0(long ptr, int idx);

    static native int rawget0(long ptr, int idx);

    static native int getMetatable0(long ptr, int idx);


    static native int pcall0(long ptr, int nargs, int nresults, int errfunc);

    static native void close0(long ptr);

    static native void setGlobal0(long ptr, @NonNull String global);

    static native int getGlobal0(long ptr, @NonNull String global);

    static native int type0(long ptr, int idx);

    static native String typeName0(long ptr, int type);

    static native void pushNil0(long ptr);

    static native void remove0(long ptr, int idx);

    static native void createTable0(long ptr, int narr, int nrec);

    static native int getField0(long ptr, int idx, String key);

    static native void setField0(long ptr, int idx, String key);

    static native int rawGetI0(long ptr, int idx, int n);

    static native void rawSetI0(long ptr, int idx, int n);

    static native int getI0(long ptr, int idx, int n);

    static native void setI0(long ptr, int idx, int n);

    static native boolean next0(long ptr, int idx);

    static native void insert0(long ptr,int idx);


    //lauxlib.h
    static native int loadBufferx0(long ptr, @NonNull byte[] buffer,
                                   @Nullable String name,
                                   @Nullable String mode);

    static native void openLibs0(long ptr);

    static native long checkInteger0(long ptr, int arg);

    static native String checkLString0(long ptr, int arg);


    static native void checkType0(long ptr, int arg, int t);

    static native int ref0(long ptr, int t);

    static native void unref0(long ptr, int t, int ref);

}
