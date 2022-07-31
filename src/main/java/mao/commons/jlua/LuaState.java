package mao.commons.jlua;

import androidx.annotation.NonNull;

import java.io.Closeable;

public class LuaState implements Closeable {


    long ptr;

    protected LuaState(long ptr) {
        this.ptr = ptr;
    }

    public static LuaState create() {
        return new FinalizeLuaState();
    }

    static LuaState wrap(long ptr) {
        if (ptr == 0) {
            throw new IllegalArgumentException("ptr is null");
        }
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

    public void remove(int idx) {
        LuaJNI.remove0(ptr, idx);
    }

    public void pushInt32(int i) {
        LuaJNI.pushInteger0(ptr, i);
    }

    public void pushInt64(long i) {
        LuaJNI.pushInteger0(ptr, i);
    }


    public int toInt32(int idx) {
        return (int) LuaJNI.toInteger0(ptr, idx);
    }

    public long toInt64(int idx) {
        return LuaJNI.toInteger0(ptr, idx);
    }

    public boolean isInteger(int idx) {
        return LuaJNI.isInteger0(ptr, idx);
    }

    public void pushBoolean(boolean b) {
        LuaJNI.pushBoolean0(ptr, b);
    }

    public boolean toBoolean(int idx) {
        return LuaJNI.toBoolean0(ptr, idx);
    }

    public void pushNumber(double num) {
        LuaJNI.pushNumber0(ptr, num);
    }

    public double toNumber(int idx) {
        return LuaJNI.toNumber0(ptr, idx);
    }

    public void call(int nargs, int nresults) {
        LuaJNI.call0(ptr, nargs, nresults);
    }

    public int pcall(int nargs, int nresults, int errfunc) {
        return LuaJNI.pcall0(ptr, nargs, nresults, errfunc);
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

    public int type(int idx) {
        return LuaJNI.type0(ptr, idx);
    }

    public String typeName(int type) {
        return LuaJNI.typeName0(ptr, type);
    }

    public void pushNil() {
        LuaJNI.pushNil0(ptr);
    }

    public boolean isNil(int idx) {
        return LuaJNI.type0(ptr, idx) == LuaJNI.LUA_TNIL;
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
        LuaJNI.loadBufferx0(ptr, buf.getBytes(), null, null);
    }

    public void setGlobal(String global) {
        LuaJNI.setGlobal0(ptr, global);
    }

    public void getGlobal(String global) {
        LuaJNI.getGlobal0(ptr, global);
    }

    public int getI(int idx, int n) {
        return LuaJNI.getI0(ptr, idx, n);
    }

    public void setI(int idx, int n) {
        LuaJNI.setI0(ptr, idx, n);
    }

    public int getField(int idx, String key) {
        return LuaJNI.getField0(ptr, idx, key);
    }

    public void setField(int idx, String key) {
        LuaJNI.setField0(ptr, idx, key);
    }

    public int rawGetI(int idx, int n) {
        return LuaJNI.rawGetI0(ptr, idx, n);
    }

    public void rawSetI(int idx, int n) {
        LuaJNI.rawSetI0(ptr, idx, n);
    }

    public void newTable() {
        LuaJNI.createTable0(ptr, 0, 0);
    }

    public void checkType(int arg, int t) {
        LuaJNI.checkType0(ptr, arg, t);
    }

    @Override
    public void close() {
    }

    public int ref(int t) {
        return LuaJNI.ref0(ptr, t);
    }

    public void unref(int t, int ref) {
        LuaJNI.unref0(ptr, t, ref);
    }


    static final class FinalizeLuaState extends LuaState {

        FinalizeLuaState() {
            super(LuaJNI.newState0());
        }

        @Override
        public void close() {
            synchronized (this) {
                if (ptr != 0) {
                    LuaJNI.close0(ptr);
                    ptr = 0;
                }
            }
        }

        @Override
        protected void finalize() throws Throwable {
            close();
        }
    }
}
