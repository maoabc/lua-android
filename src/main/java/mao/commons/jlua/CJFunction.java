package mao.commons.jlua;

import dalvik.annotation.optimization.FastNative;

/**
 * java层接口传递到native,通过ffi生成c闭包
 * 对应结构体:
 * {
 * ffi_closure closure;
 * ffi_cif cif;
 * ffi_type *args[1];
 * lua_CFunction func;
 * <p>
 * }
 */
public abstract class CJFunction implements JFunction {
    //native层结构体指针
    private long ptr;

    public CJFunction() {
        this.ptr = createClosure0(this);
    }

    @Override
    public final int call(long luaState) throws Throwable {
        return call(LuaState.wrap(luaState));
    }

    @Override
    protected void finalize() throws Throwable {
        synchronized (this) {
            if (ptr != 0) {
                freeClosure0(ptr);
                ptr = 0;
            }
        }
    }

    long getCFunction() {
        return getCFunction0(ptr);
    }

    protected abstract int call(LuaState luaState) throws Throwable;





    private static native long createClosure0(JFunction function);

    @FastNative
    private static native long getCFunction0(long ptr);

    @FastNative
    private static native JFunction getJFunction0(long ptr);

    private static native void freeClosure0(long ptr);
}
