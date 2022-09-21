package mao.commons.jlua;


import androidx.annotation.Size;


/**
 * 栈顶是一个tab,包含java方法名及对应的lua回调函数,
 * 生成一个全新luaState用来调用回调函数
 */
public class LuaCallbackContext {
    private int ref;
    public final LuaState l;

    public LuaCallbackContext(LuaState l) {
        final long[] newL = new long[1];
        ref = newContext0(l.ptr, newL);
        this.l = LuaState.wrap(newL[0]);
    }

    public void testExec() {
        final int type2 = l.type(1);
        final int type = l.type(-1);
        final int top = l.getTop();
        l.getField(-1, "callback");
        final int type1 = l.type(-1);
        System.out.println();
    }



    private static native int newContext0(long ptr, @Size(1) long[] newPtr);


    @Override
    protected void finalize() throws Throwable {
        synchronized (this) {
            if (ref != LuaState.LUA_NOREF) {
                l.unref(LuaJNI.LUA_REGISTRYINDEX, ref);
                ref = LuaState.LUA_NOREF;
            }
        }
    }
}
