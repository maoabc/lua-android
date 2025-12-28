package mao.commons.jlua;


import java.io.Closeable;

/**
 * 栈顶是一个table或者函数, table需要包含java方法名及对应的lua回调函数,
 * 生成一个全新luaState用来调用回调函数
 * 生命周期跟随java对象
 */
public final class LuaCallbackContext implements Closeable {
    //对应到c的结构体callback_context
    private int ref;
    public final LuaState l;


    public LuaCallbackContext(LuaState luaState) {
        final int type = luaState.type(-1);
        if (type != LuaState.LUA_TTABLE && type != LuaState.LUA_TFUNCTION) {
            throw new LuaException("not a table or function");
        }
        final long[] newL = new long[1];
        ref = LuaJNI.newContext0(luaState.ptr, newL);
        this.l = LuaState.wrap(newL[0]);
        //栈底放入错误处理函数
        l.pushCFunction(UtilFunctions.traceback());
        l.insert(1);

    }


    @Override
    public void close() {
        synchronized (this) {
            if (ref != LuaState.LUA_NOREF) {
                l.unref(LuaJNI.LUA_REGISTRYINDEX, ref);
                ref = LuaState.LUA_NOREF;
            }
        }
    }

    @Override
    protected void finalize() throws Throwable {
        close();
    }
}
