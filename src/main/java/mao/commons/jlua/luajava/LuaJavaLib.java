package mao.commons.jlua.luajava;

import mao.commons.jlua.CJFunction;
import mao.commons.jlua.LuaState;

public class LuaJavaLib {
    private final CJFunction javaBindClassFunction = new CJFunction() {
        @Override
        protected int call(LuaState luaState) throws Throwable {
            final String className = luaState.checkLString(-1);
            final Class<?> clazz = Class.forName(className);
            new ReflectionObject(clazz, luaState);
            return 1;
        }
    };
    private final CJFunction javaNewFunction = new CJFunction() {
        @Override
        protected int call(LuaState luaState) {
            return 0;
        }
    };

    private final CJFunction javaNewInstanceFunction = new CJFunction() {
        @Override
        protected int call(LuaState luaState) {
            return 0;
        }
    };

    private final CJFunction loadLibFunction = new CJFunction() {
        @Override
        protected int call(LuaState luaState) {
            return 0;
        }
    };

    private final CJFunction createProxyFunction = new CJFunction() {
        @Override
        protected int call(LuaState luaState) {
            return 0;
        }
    };

    private final CJFunction metaIndexFunction = new CJFunction() {
        @Override
        protected int call(LuaState luaState) {
            final String name = luaState.checkLString(-1);
            switch (name) {
                case "bindClass":
                    luaState.pushFunction(javaBindClassFunction);
                    return 1;
                case "new":
                    luaState.pushFunction(javaNewFunction);
                    return 1;
                default:
                    //todo 异常
                    return 0;
            }
        }
    };


    public LuaJavaLib(LuaState l) {
        l.pushJavaObject(this);
        l.setGlobal("luajava");
        //得到对象
        l.getGlobal("luajava");

        l.getMetatable(-1);

        l.pushString("__index");
        l.pushFunction(metaIndexFunction);
        l.setTable(-3);

        l.pop(2);

    }
}
