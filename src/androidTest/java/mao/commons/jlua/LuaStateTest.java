package mao.commons.jlua;


import org.junit.Test;

public class LuaStateTest {

    @Test
    public void create() {
        final LuaState state = LuaState.create();
        final CJFunction function = new CJFunction() {
            @Override
            protected int call(LuaState luaState) {
                final int i1 = luaState.checkInteger(-1);
                final int i2 = luaState.checkInteger(-2);
                luaState.pushInteger(i1 + i2);
                return 1;
            }
        };
        state.pushClosure(function, 0);
        state.pushInteger(4);
        state.pushInteger(5);
        state.call(2, 1);
        final int i = state.toInteger(1);
        state.setGlobal("myv");
        state.getGlobal("myv");
        final int i1 = state.toInteger(-1);
        state.loadBuffer("function abc(a) return a*a end");
        state.call(0, 0);
        state.getGlobal("abc");
        state.pushInteger(4);
        state.call(1, 1);
        final int i2 = state.toInteger(-1);

        System.out.println(i);
    }

    @Test
    public void testJavaObject() {
        final LuaState state = LuaState.create();
        String s = "Hello world";
        state.pushJavaObject(s);

        state.getMetatable(-1);

        final CJFunction indexFunc = new CJFunction() {
            @Override
            protected int call(LuaState luaState) {
                final boolean javaObject = luaState.isJavaObject(-2);
                final Object object = luaState.toJavaObject(-2);
                final boolean string = luaState.isString(-1);
                final String methodName = luaState.toLString(-1);
                if ("toString".equals(methodName)) {
                    final CJFunction tostring = new CJFunction() {
                        @Override
                        protected int call(LuaState luaState) {
                            luaState.pushString("xxx");
                            return 1;
                        }
                    };
                    luaState.pushClosure(tostring, 1);
                }
                return 1;
            }
        };
        state.pushString("__index");
        state.pushClosure(indexFunc, 0);
        state.rawSet(-3);
        state.setTop(1);

        state.setGlobal("mys");

        state.loadBuffer("local x=mys.toString()");
        state.call(0, 0);


        final boolean b = state.isJavaObject(-1);
        System.out.println(b);

    }
}