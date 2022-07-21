package mao.commons.jlua;

import androidx.annotation.NonNull;
import androidx.annotation.Nullable;

public class LuaJNI {

    static {
        System.loadLibrary("jlua");
    }

    //lua.h
    static native long newState0();


    static native void pushInteger0(long ptr, int i);

    static native int toInteger0(long ptr, int idx);

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

    static native void call0(long ptr, int nargs, int nresults);

    static native void pcall0(long ptr, int nargs, int nresults, int errfunc);

    static native void close0(long ptr);

    static native void setGlobal0(long ptr, @NonNull String global);

    static native int getGlobal0(long ptr, @NonNull String global);

    static native int error0(long ptr);




    //lauxlib.h
    static native int loadBufferx0(long ptr, @NonNull String buffer,
                                   @Nullable String name,
                                   @Nullable String mode);

    static native int checkInteger0(long ptr, int arg);

    static native String checkLString0(long ptr, int arg);

}
