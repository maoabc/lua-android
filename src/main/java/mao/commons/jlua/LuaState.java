package mao.commons.jlua;

import java.io.Closeable;

public class LuaState implements Closeable {

    static {
        System.loadLibrary("jlua");
    }

    private long ptr;
    private final boolean needRelease;

    private LuaState() {
        this.ptr = newState0();
        needRelease = true;
    }

    private LuaState(long ptr) {
        this.ptr = ptr;
        needRelease = false;
    }

    public static LuaState create() {
        return new LuaState();
    }

    public static LuaState wrap(long ptr) {
        return new LuaState(ptr);
    }

    public int checkInteger(int arg) {
        return checkInteger0(ptr, arg);
    }

    public void pushClosure(CJFunction function, int n) {
        pushClosure0(ptr, function.getCFunction(), n);
    }

    public void pushInteger(int i) {
        pushInteger0(ptr, i);
    }

    public int toInteger(int idx) {
        return toInteger0(ptr, idx);
    }

    public void call(int nargs, int nresults) {
        call0(ptr, nargs, nresults);
    }

    @Override
    public void close() {
        release();
    }

    @Override
    protected void finalize() throws Throwable {
        release();
    }

    private void release() {
        if (needRelease) {
            synchronized (this) {
                if (ptr != 0) {
                    close0(ptr);
                    ptr = 0;
                }
            }
        }
    }

    private static native long newState0();


    private static native int checkInteger0(long ptr, int arg);

    private static native void pushInteger0(long ptr, int i);

    private static native int toInteger0(long ptr, int idx);

    private static native void pushClosure0(long ptr, long funcPtr, int n);

    private static native void call0(long ptr, int nargs, int nresults);

    private static native void close0(long ptr);
}
