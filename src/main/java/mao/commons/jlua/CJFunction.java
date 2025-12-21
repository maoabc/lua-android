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
public abstract class CJFunction {
    //native层结构体指针
    private long ptr;

    public CJFunction() {
        this.ptr = createClosure0(this);
    }

    //被native层调用
    public final int call(long luaState) throws Throwable {
        return call(LuaState.wrap(luaState));
    }

    @Override
    protected final void finalize() throws Throwable {
        synchronized (this) {
            if (ptr != 0) {
                freeClosure0(ptr);
                ptr = 0;
            }
        }
    }

    final long getCFunction() {
        if (ptr == 0) {
            return UtilFunctions.emptyFunc();
        }
        return getCFunction0(ptr);
    }

    protected abstract int call(LuaState luaState) throws Throwable;


    private static native long createClosure0(CJFunction function);

    @FastNative
    private static native long getCFunction0(long ptr);

    @FastNative
    private static native CJFunction getJFunction0(long ptr);

    private static native void freeClosure0(long ptr);
}
