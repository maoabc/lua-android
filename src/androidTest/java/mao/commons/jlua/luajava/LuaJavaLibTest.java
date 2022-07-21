package mao.commons.jlua.luajava;


import org.junit.Test;

import mao.commons.jlua.LuaState;

public class LuaJavaLibTest {

    @Test
    public void testLib() throws InterruptedException {
        final LuaState luaState = LuaState.create();
        new LuaJavaLib(luaState);
        luaState.loadBuffer("local cls=luajava.bindClass(\"java.lang.String\")\n" +
                "b=cls.getClass()\n" +
                "\n"+
                "");
        luaState.call(0, 0);
        luaState.close();
//        Thread.sleep(40000);
    }
}