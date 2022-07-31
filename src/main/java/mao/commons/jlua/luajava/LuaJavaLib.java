package mao.commons.jlua.luajava;


import java.lang.reflect.Method;
import java.lang.reflect.Proxy;

import mao.commons.jlua.CJFunction;
import mao.commons.jlua.LuaCallbackContext;
import mao.commons.jlua.LuaException;
import mao.commons.jlua.LuaState;

public class LuaJavaLib {
    private final CJFunction bindClassFunction = new CJFunction() {
        @Override
        protected int call(LuaState luaState) throws Throwable {
            final String className = luaState.checkLString(-1);
            final Class<?> clazz = Class.forName(className);
            ReflectionObject.registerObject(luaState, clazz);
            return 1;
        }
    };
    private final CJFunction newFunction = new CJFunction() {
        @Override
        protected int call(LuaState luaState) throws Throwable {
            final Class<?> clazz;

            if (luaState.isJavaObject(1)) {
                clazz = (Class<?>) luaState.checkJavaObject(1);
            } else if (luaState.isString(1)) {
                clazz = Class.forName(luaState.checkLString(1));
            } else {
                luaState.pushString("Not java class or class name");
                luaState.error();
                return 0;
            }
            final Object newObj = ReflectionUtils.invokeConstructor(luaState, clazz, luaState.getTop() - 1);

            ReflectionObject.registerObject(luaState, newObj);

            return 1;
        }
    };

    private final CJFunction loadLibFunction = new CJFunction() {
        @Override
        protected int call(LuaState l) throws Throwable {
            final String className = l.checkLString(1);
            final String methodName = l.checkLString(2);

            final Class<?> clazz = Class.forName(className);

            try {
                Method method = clazz.getMethod(methodName, LuaState.class);
                Object obj = method.invoke(null, l);

                if (obj instanceof Integer) {
                    return (Integer) obj;
                }
                return 0;
            } catch (Exception e) {
                throw new LuaException("Library could not be loaded. " + e.getMessage());
            }
        }
    };

    private final CJFunction createProxyFunction = new CJFunction() {
        @Override
        protected int call(LuaState luaState) throws Throwable {
            final int top = luaState.getTop();
            if (top < 2) {
                //todo 抛出异常
            }

            final String[] interfaces = luaState.checkLString(1).split(",");

            luaState.remove(1);
            final int top1 = luaState.getTop();

            final Class<?>[] classes = new Class[interfaces.length];
            for (int i = 0; i < interfaces.length; i++) {
                classes[i] = Class.forName(interfaces[i]);
            }
            final LuaCallbackContext context = new LuaCallbackContext(luaState);
            final Object proxyInstance = Proxy.newProxyInstance(LuaJavaLib.class.getClassLoader(), classes, new MyInvocationHandler(context));
            ReflectionObject.registerObject(luaState, proxyInstance);
            return 1;
        }
    };

    private final CJFunction metaIndexFunction = new CJFunction() {
        @Override
        protected int call(LuaState luaState) {
            final String name = luaState.checkLString(-1);
            switch (name) {
                case "bindClass":
                    luaState.pushFunction(bindClassFunction);
                    return 1;
                case "new":
                case "newInstance":
                    luaState.pushFunction(newFunction);
                    return 1;
                case "loadLib":
                    luaState.pushFunction(loadLibFunction);
                    return 1;
                case "createProxy":
                    luaState.pushFunction(createProxyFunction);
                    return 1;

                default:
                    throw new LuaException("Not found function " + name);
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
