package mao.commons.jlua.luajava;


import java.lang.reflect.Field;
import java.lang.reflect.Method;

import mao.commons.jlua.CJFunction;
import mao.commons.jlua.LuaState;

/**
 * 通过反射执行的java对象
 */
public class ReflectionObject {
    private final Object obj;

    private static final CJFunction indexFunc = new CJFunction() {
        @Override
        protected int call(LuaState luaState) {
            final Object objOrClazz = ((ReflectionObject) luaState.checkJavaObject(-2)).obj;
            final String name = luaState.checkLString(-1);
            //匹配field,且读取对应的值
            final Field field = ReflectionUtils.getField(objOrClazz, name);
            if (field != null) {
                return 1;
            }
            final Method method = ReflectionUtils.getMethod(objOrClazz, name);
            if (method != null) {
                luaState.pushClosure(methodInvokeFunction, 2);
                return 1;
            }

            //找不到field或者method
            luaState.pushString(objOrClazz.getClass().getName() + "No such field or method: " + name);
            luaState.error();
            return 0;

        }
    };
    private static final CJFunction newIndexFunction = new CJFunction() {
        @Override
        protected int call(LuaState luaState) throws Throwable {
            return 0;
        }
    };

    private static final CJFunction methodInvokeFunction = new CJFunction() {
        @Override
        protected int call(LuaState luaState) throws Throwable {
            System.out.println("getclass");
            return 0;
        }
    };

    public ReflectionObject(Object object, LuaState l) {
        this.obj = object;
        l.pushJavaObject(this);

        l.getMetatable(-1);

        l.pushString("__index");
        l.pushFunction(indexFunc);
        l.rawSet(-3);

        l.pushString("__newindex");
        l.pushFunction(newIndexFunction);
        l.rawSet(-3);


        l.pop(1);

    }
}
