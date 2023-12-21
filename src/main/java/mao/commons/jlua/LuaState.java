package mao.commons.jlua;

import androidx.annotation.NonNull;

import java.io.Closeable;

public class LuaState implements Closeable {


    public static final int LUA_OK = 0;
    public static final int LUA_YIELD = 1;
    public static final int LUA_ERRRUN = 2;
    public static final int LUA_ERRSYNTAX = 3;
    public static final int LUA_ERRMEM = 4;
    public static final int LUA_ERRERR = 5;

    /*
     ** basic types
     */
    public static final int LUA_TNONE = (-1);

    public static final int LUA_TNIL = 0;
    public static final int LUA_TBOOLEAN = 1;
    public static final int LUA_TLIGHTUSERDATA = 2;
    public static final int LUA_TNUMBER = 3;
    public static final int LUA_TSTRING = 4;
    public static final int LUA_TTABLE = 5;
    public static final int LUA_TFUNCTION = 6;
    public static final int LUA_TUSERDATA = 7;
    public static final int LUA_TTHREAD = 8;

    public static final int LUA_NUMTYPES = 9;


    /* predefined references */
    public static final int LUA_NOREF = (-2);
    public static final int LUA_REFNIL = (-1);


    long ptr;

    protected LuaState(long ptr) {
        this.ptr = ptr;
    }

    public static LuaState create() {
        final FinalizeLuaState finalizeLuaState = new FinalizeLuaState();
        finalizeLuaState.openLibs();
        return finalizeLuaState;
    }

    static LuaState wrap(long ptr) {
        if (ptr == 0) {
            throw new IllegalArgumentException("ptr is null");
        }
        return new LuaState(ptr);
    }

    public void openLibs() {
        LuaJNI.openLibs0(ptr);
    }

    public int checkInt(int arg) {
        return (int) LuaJNI.checkInteger0(ptr, arg);
    }

    public long checkInt64(int arg) {
        return LuaJNI.checkInteger0(ptr, arg);
    }

    public void pushClosure(CJFunction function, int n) {
        LuaJNI.pushClosure0(ptr, function.getCFunction(), n);
    }

    public void pushFunction(CJFunction function) {
        LuaJNI.pushClosure0(ptr, function.getCFunction(), 0);
    }


    public void pushCFunction(long funcPtr) {
        LuaJNI.pushClosure0(ptr, funcPtr, 0);
    }

    public void pushCClosure(long funcPtr, int n) {
        LuaJNI.pushClosure0(ptr, funcPtr, n);
    }

    public void remove(int idx) {
        LuaJNI.remove0(ptr, idx);
    }

    public void insert(int idx) {
        LuaJNI.insert0(ptr, idx);
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
        if (LuaJNI.pcall0(ptr, nargs, nresults, 1) != LUA_OK) {
            String traceback = "";
            if (isString(-1)) {
                traceback = toLString(-1);
            }
            throw new LuaException(traceback);
        }
    }

    public int pcall(int nargs, int nresults, int errfunc) {
        return LuaJNI.pcall0(ptr, nargs, nresults, errfunc);
    }

    public static int upValueIndex(int idx) {
        return LuaJNI.LUA_REGISTRYINDEX - idx;
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

    public void pushBytes(@NonNull byte[] bytes) {
        LuaJNI.pushBytes0(ptr, bytes);
    }

    public void pushValue(int idx) {
        LuaJNI.pushValue0(ptr, idx);
    }

    public boolean isString(int idx) {
        return LuaJNI.isString0(ptr, idx);
    }

    public boolean isFunction(int idx) {
        return LuaJNI.type0(ptr, idx) == LUA_TFUNCTION;
    }

    public boolean isTable(int idx) {
        return LuaJNI.type0(ptr, idx) == LUA_TTABLE;
    }

    public void iterTable(TableIterator iterator) {
        pushNil();
        while (next(-2)) {
            if (iterator != null) iterator.iter(this);
            pop(1);
        }
    }

    public String checkString(int arg) {
        return LuaJNI.checkLString0(ptr, arg);
    }

    public String optString(int idx, String def) {
        if (type(idx) <= 0) {
            return def;
        }
        return checkString(idx);
    }

    public int optInt(int idx, int def) {
        if (type(idx) <= 0) {
            return def;
        }
        return checkInt(idx);
    }

    public boolean optBoolean(int idx, boolean def) {
        if (type(idx) <= 0) {
            return def;
        }
        return toBoolean(idx);
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
        return LuaJNI.type0(ptr, idx) == LUA_TNIL;
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
        loadBuffer(buf, null);
    }

    public void loadBuffer(String buf, String name) {
        if (LuaJNI.loadBufferx0(ptr, buf.getBytes(), name, null) != LUA_OK) {
            throw new LuaException(checkString(-1));
        }
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

    public boolean next(int idx) {
        return LuaJNI.next0(ptr, idx);
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


    private static final class FinalizeLuaState extends LuaState {

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

    public interface TableIterator {
        void iter(LuaState l);
    }
}
