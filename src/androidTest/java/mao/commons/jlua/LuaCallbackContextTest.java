package mao.commons.jlua;


import org.junit.Test;

import mao.commons.jlua.luajava.LuaJavaLib;

public class LuaCallbackContextTest {


    //保证加载native库
    static {
        try (final LuaState state = LuaState.create()) {
            state.pushNil();
        }
    }

    private CJFunction callbackFunc = new CJFunction() {
        @Override
        protected int call(LuaState luaState) throws Throwable {
            final String s = luaState.checkLString(-1);
            return 0;
        }
    };

    @Test
    public void setCallback() throws Exception {
        final LuaState luaState = LuaState.create();
        luaState.loadBuffer("local a={\n" +
                "callback=function() end\n" +
                "}\n" +
                "return a"
        );
        luaState.pcall(0, 1, 0);
        final int type = luaState.type(-1);
        final LuaCallbackContext context = new LuaCallbackContext(luaState);
        context.testExec();
    }

    @Test
    public void testCreateProxy() {
        final LuaState luaState = LuaState.create();
        new LuaJavaLib(luaState);

        luaState.loadBuffer("" +
                "local a={\n" +
                "print=function(s,i,f) return \"lua return\"end\n" +
                ",ddd=function() return \"ttyt\" end\n"+
                ",fff=function() end\n"+
                "}\n" +
                "o=luajava.createProxy(\"mao.commons.jlua.TT\",a)\n" +
                "return o"
        );

        luaState.pcall(0, 1, 0);
        final boolean javaObject = luaState.isJavaObject(-1);
        final Object obj = luaState.checkJavaObject(-1);

        Integer a = 556;

        if (obj instanceof TT) {
//            final String ssss = ((TT) obj).print("ssss", 56, 66);
//            System.out.println();
            final int ddd = ((TT) obj).ddd();
            System.out.println();
        }
        System.out.println();
    }
}