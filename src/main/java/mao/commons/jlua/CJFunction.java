package mao.commons.jlua;

import dalvik.annotation.optimization.FastNative;

/**
 * 需要注意生命周期，一般java层的函数对象为静态或者作为其他对象的实例域，
 * 千万不能写在方法中作为局部变量，因为native层持有对象的弱引用，一旦java层
 * 无引用,对象就会被回收从而导致native层引用失效
 * <p>
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

    private final long funcPtr;

    public CJFunction() {
        this.ptr = createClosure0(this);
        if (ptr == 0) {
            throw new IllegalStateException("function create error");
        }
        this.funcPtr = getCFunction0(ptr);
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
        //没必要每次从native层获取
        return funcPtr;
    }

    protected abstract int call(LuaState luaState) throws Throwable;


    @FastNative
    private static native long createClosure0(CJFunction function);

    @FastNative
    private static native long getCFunction0(long ptr);

    @FastNative
    private static native void freeClosure0(long ptr);
}
