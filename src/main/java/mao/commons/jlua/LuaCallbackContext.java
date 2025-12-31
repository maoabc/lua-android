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
        //生成thread对象，同时把它设置为userdata的value
        final long newL = LuaJNI.newContext0(luaState.ptr);
        l = LuaState.wrap(newL);

        //userdata 放入私有全局空间，通过ref索引管理它
        ref = l.ref(LuaJNI.LUA_REGISTRYINDEX);

        //栈底放入错误处理函数
        l.pushCFunction(UtilFunctions.traceback());
        l.insert(1);

        //现在用于回调的luaState栈底是错误处理函数，栈顶是回调函数或者table(table中多个回调函数）

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
