package mao.commons.jlua;

import mao.commons.jlua.luajava.LuaJavaLib;

/**
 * 自定义资源加载等
 */
public class LuaAndroid {

    /**
     * 新建一个自定义资源查找器的luaState
     *
     * @param finder
     * @return
     */
    public static LuaState newLuaState(ResourceFinder finder) {
        final LuaState luaState = LuaState.create();
        luaState.pushCFunction(FunctionUtils.traceback());
        setup(luaState, finder);
        new LuaJavaLib(luaState);
        return luaState;
    }

    public static void setup(LuaState l, ResourceFinder finder) {
        setup0(l.ptr, finder);
    }

    //初始化资源查找器
    private static native void setup0(long ptr, ResourceFinder finder);
}
