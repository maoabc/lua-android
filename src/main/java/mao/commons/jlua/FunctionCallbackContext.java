package mao.commons.jlua;


import androidx.annotation.Size;


/**
 * 栈顶是lua函数,把栈顶函数放在新的luaState栈顶,栈底放置调用错误跟踪函数
 * 生成一个全新luaState用来调用回调函数
 * 生命周期跟随java对象
 */
public class FunctionCallbackContext {
    //对应到c的结构体callback_context
    private int contextRef;
    public final LuaState l;

    public FunctionCallbackContext(LuaState l) {
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
