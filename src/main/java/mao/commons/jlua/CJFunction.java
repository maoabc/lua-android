package mao.commons.jlua;

/**
 * java层接口传递到native,通过ffi生成c闭包
 */
public abstract class CJFunction implements JFunction {
    private long ptr;

    public CJFunction() {
        this.ptr = createClosure0(this);
    }

    @Override
    public final int call(long luaState) {
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

    protected abstract int call(LuaState luaState);

    private static native long createClosure0(JFunction function);

    private static native long getCFunction0(long ptr);

    private static native JFunction getJFunction0(long ptr);

    private static native void freeClosure0(long ptr);
}
