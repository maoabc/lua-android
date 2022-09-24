package mao.commons.jlua;


import androidx.annotation.Size;


/**
 * 栈顶是一个tab或者函数,包含java方法名及对应的lua回调函数,
 * 生成一个全新luaState用来调用回调函数
 * 生命周期跟随java对象
 */
public class LuaCallbackContext {
    //对应到c的结构体callback_context
    private int contextRef;
    public final LuaState l;

    public LuaCallbackContext(LuaState l) {
        this(l, true);
    }

    public LuaCallbackContext(LuaState l, boolean proxy) {
        final long[] newL = new long[1];
        if (proxy) {
            if (l.type(-1) != LuaState.LUA_TTABLE) {
                throw new LuaException("not a table");
            }
        } else {
            if (l.type(-1) != LuaState.LUA_TFUNCTION) {
                throw new LuaException("not a function");
            }
        }
        contextRef = newContext0(l.ptr, newL);
        this.l = LuaState.wrap(newL[0]);
    }


    private static native int newContext0(long ptr, @Size(1) long[] newPtr);


    @Override
    protected void finalize() throws Throwable {
        synchronized (this) {
            if (contextRef != LuaState.LUA_NOREF) {
                l.unref(LuaJNI.LUA_REGISTRYINDEX, contextRef);
                contextRef = LuaState.LUA_NOREF;
            }
        }
    }
}
