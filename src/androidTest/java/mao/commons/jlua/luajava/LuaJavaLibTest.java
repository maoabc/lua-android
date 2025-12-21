package mao.commons.jlua.luajava;


import org.junit.Test;

import java.lang.reflect.Array;

import mao.commons.jlua.CJFunction;
import mao.commons.jlua.LuaState;

public class LuaJavaLibTest {

    @Test
    public void testLib() throws InterruptedException {
        final LuaState luaState = LuaState.create();
        LuaJavaLib.register(luaState);
        luaState.loadBuffer("local cls=luajava.bindClass(\"java.lang.String\")\n" +
                "b=cls.getClass\n" +
                "我=luajava.new(cls,\"dsdlkjdslkj\")\n" +
//                "b()\n"+
                "ff=cls:format(\"hello %s %s\",cls,\"ddd\")\n" +
                "return 我");
        final int[] ints = new int[3];
        final Object[] objects = new Object[3];
        final boolean array = ints.getClass().isArray();
        final boolean b = ints instanceof int[];
        final CJFunction err = new CJFunction() {
            @Override
            protected int call(LuaState luaState) throws Throwable {
                return 0;
            }
        };
        luaState.pcall(0, 1, 0);
        final Object s = luaState.toJavaObject(-1);
        luaState.pushString("dddd");
        luaState.setGlobal("我");
        luaState.getGlobal("我");
        final String s1 = luaState.toLString(-1);
        luaState.close();
//        Thread.sleep(40000);
    }
}