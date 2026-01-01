package mao.commons.jlua;

import dalvik.annotation.optimization.CriticalNative;

/**
 * 使用CriticalNative优化的本地方法，因为不能向后兼容所以独立出来
 */
class FastLuaJNI {

    //lua.h

    @CriticalNative
    static native void pushInteger0(long ptr, long i);

    @CriticalNative
    static native long toInteger0(long ptr, int idx);

    @CriticalNative
    static native boolean isInteger0(long ptr, int idx);

    @CriticalNative
    static native boolean isFloat0(long ptr, int idx);

    @CriticalNative
    static native void pushBoolean0(long ptr, boolean b);

    @CriticalNative
    static native boolean isBoolean0(long ptr, int idx);

    @CriticalNative
    static native boolean toBoolean0(long ptr, int idx);

    @CriticalNative
    static native void pushNumber0(long ptr, double num);

    @CriticalNative
    static native boolean isNumber0(long ptr, int idx);

    @CriticalNative
    static native double toNumber0(long ptr, int idx);

    @CriticalNative
    static native void pushClosure0(long ptr, long funcPtr, int n);

    @CriticalNative
    static native void pushValue0(long ptr, int idx);

    @CriticalNative
    static native boolean isString0(long ptr, int idx);

    @CriticalNative
    static native void setTop0(long ptr, int idx);

    @CriticalNative
    static native int getTop0(long ptr);

    @CriticalNative
    static native int type0(long ptr, int idx);

    @CriticalNative
    static native void pushNil0(long ptr);

    @CriticalNative
    static native void remove0(long ptr, int idx);

    @CriticalNative
    static native void rotate0(long ptr, int idx, int n);

    @CriticalNative
    static native long findRootThread0(long ptr);



}
