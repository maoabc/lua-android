package mao.commons.jlua;


import androidx.annotation.Size;


/**
 * 栈顶是一个tab,包含java方法名及对应的lua回调函数,
 * 生成一个全新luaState用来调用回调函数
 */
public class LuaJavaCallbackContext {
    //对应到c的结构体callback_context
    private int contextRef;
    public final LuaState l;

    public LuaJavaCallbackContext(LuaState l) {
        final long[] newL = new long[1];
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
