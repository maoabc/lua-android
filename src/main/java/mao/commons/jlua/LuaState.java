package mao.commons.jlua;

import androidx.annotation.NonNull;

import java.io.Closeable;

public class LuaState implements Closeable {


    private long ptr;
    private final boolean needRelease;

    private LuaState() {
        this.ptr = LuaJNI.newState0();
        needRelease = true;
    }

    private LuaState(long ptr) {
        this.ptr = ptr;
        needRelease = false;
    }

    public static LuaState create() {
        return new LuaState();
    }

    static LuaState wrap(long ptr) {
        return new LuaState(ptr);
    }

    public int checkInteger(int arg) {
        return LuaJNI.checkInteger0(ptr, arg);
    }

    public void pushClosure(CJFunction function, int n) {
        LuaJNI.pushClosure0(ptr, function.getCFunction(), n);
    }

    public void pushFunction(CJFunction function) {
        LuaJNI.pushClosure0(ptr, function.getCFunction(), 0);
    }

    public void pushInteger(int i) {
        LuaJNI.pushInteger0(ptr, i);
    }

    public int toInteger(int idx) {
        return LuaJNI.toInteger0(ptr, idx);
    }

    public void call(int nargs, int nresults) {
        LuaJNI.call0(ptr, nargs, nresults);
    }

    public void pushJavaObject(Object obj) {
        LuaJNI.pushJavaObject0(ptr, obj);
    }

    public Object toJavaObject(int idx) {
        return LuaJNI.toJavaObject0(ptr, idx);
    }

    public Object checkJavaObject(int idx) {
        return LuaJNI.checkJavaObject0(ptr, idx);
    }

    public void pushString(@NonNull String str) {
        LuaJNI.pushString0(ptr, str);
    }

    public boolean isString(int idx) {
        return LuaJNI.isString0(ptr, idx);
    }

    public String checkLString(int arg) {
        return LuaJNI.checkLString0(ptr, arg);
    }

    public void error() {
        LuaJNI.error0(ptr);
    }

    public String toLString(int idx) {
        return LuaJNI.toLString0(ptr, idx);
    }

    public void setTop(int idx) {
        LuaJNI.setTop0(ptr, idx);
    }

    public void pop(int n) {
        LuaJNI.setTop0(ptr, -n - 1);
    }

    public int getTop() {
        return LuaJNI.getTop0(ptr);
    }

    public boolean isJavaObject(int idx) {
        return LuaJNI.isJavaObject0(ptr, idx);
    }

    public void setTable(int idx) {
        LuaJNI.setTable0(ptr, idx);
    }

    public int getTable(int idx) {
        return LuaJNI.getTable0(ptr, idx);
    }

    public void rawSet(int idx) {
        LuaJNI.rawset0(ptr, idx);
    }

    public void rawGet(int idx) {
        LuaJNI.rawget0(ptr, idx);
    }

    public void getMetatable(int objIdx) {
        LuaJNI.getMetatable0(ptr, objIdx);
    }

    public void loadBuffer(String buf) {
        LuaJNI.loadBufferx0(ptr, buf, null, null);
    }

    public void setGlobal(String global) {
        LuaJNI.setGlobal0(ptr, global);
    }

    public void getGlobal(String global) {
        LuaJNI.getGlobal0(ptr, global);
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
                    LuaJNI.close0(ptr);
                    ptr = 0;
                }
            }
        }
    }

}
