package mao.commons.jlua.luajava;


import java.lang.reflect.Array;
import java.lang.reflect.Field;
import java.lang.reflect.InvocationTargetException;
import java.lang.reflect.Method;

import mao.commons.jlua.CJFunction;
import mao.commons.jlua.LuaState;

/**
 * 通过反射执行的java对象的方法等
 */
public class ReflectionObject {

    private static final CJFunction indexFunc = new CJFunction() {
        @Override
        protected int call(LuaState luaState) throws Throwable {
            final Object obj = luaState.checkJavaObject(-2);
            final String name = luaState.checkLString(-1);

            //匹配field,且读取对应的值
            final Field field = ReflectionUtils.getField(obj, name);
            if (field != null) {
                final Class<?> type = field.getType();
                //基本类型
                if (type == int.class) {
                    luaState.pushInt64(field.getInt(obj));
                } else if (type == boolean.class) {
                    luaState.pushBoolean(field.getBoolean(obj));
                } else if (type == byte.class) {
                    luaState.pushInt64(field.getByte(obj));
                } else if (type == char.class) {
                    luaState.pushInt64(field.getChar(obj));
                } else if (type == short.class) {
                    luaState.pushInt64(field.getShort(obj));
                } else if (type == long.class) {
                    luaState.pushInt64(field.getLong(obj));
                } else if (type == float.class) {
                    luaState.pushNumber(field.getFloat(obj));
                } else if (type == double.class) {
                    luaState.pushNumber(field.getDouble(obj));
                } else {
                    //对象类型
                    final Object value = field.get(obj);
                    if (value == null) {
                        luaState.pushNil();
                    } else if (value instanceof Integer) {
                        luaState.pushInt64((Integer) value);
                    } else if (value instanceof String) {
                        luaState.pushString((String) value);
                    } else if (value instanceof Boolean) {
                        luaState.pushBoolean((Boolean) value);
                    } else if (value instanceof Byte) {
                        luaState.pushInt64((Byte) value);
                    } else if (value instanceof Short) {
                        luaState.pushInt64((Short) value);
                    } else if (value instanceof Character) {
                        luaState.pushInt64((Character) value);
                    } else if (value instanceof Long) {
                        luaState.pushInt64((Long) value);
                    } else if (value instanceof Float) {
                        luaState.pushNumber((Float) value);
                    } else if (value instanceof Double) {
                        luaState.pushNumber((Double) value);
                    } else {
                        ReflectionObject.registerObject(luaState, value);
                    }
                }
                return 1;
            }
            //查找同名方法
            if (ReflectionUtils.hasMethod(obj, name)) {//包含对象及方法名的闭包
                luaState.pushClosure(methodInvokeFunction, 1);
                return 1;
            }

            //找不到field或者method
            luaState.pushString(obj.getClass().getName() + "No such field or method: " + name);
            luaState.error();
            return 0;

        }
    };
    private static final CJFunction newIndexFunc = new CJFunction() {
        @Override
        protected int call(LuaState luaState) throws Throwable {
            return 0;
        }
    };

    private static final CJFunction methodInvokeFunction = new CJFunction() {
        @Override
        protected int call(LuaState luaState) throws Throwable {
            final String name = luaState.checkLString(LuaState.upValueIndex(1));
            //第一个参数为对象
            final Object obj = luaState.checkJavaObject(1);
            try {
                final Object ret = invoke(luaState, obj, name, luaState.getTop() - 1);
                if (ret instanceof String) {
                    luaState.pushString((String) ret);
                }
                return 1;
            } catch (Exception e) {
                luaState.pushString(e.toString());
                luaState.error();
            }
            return 0;
        }
    };


    public static void registerObject(LuaState l, Object obj) {

        l.pushJavaObject(obj);

        l.getMetatable(-1);

        l.pushString("__index");
        l.pushFunction(indexFunc);
        l.rawSet(-3);

        l.pushString("__newindex");
        l.pushFunction(newIndexFunc);
        l.rawSet(-3);


        l.pop(1);
    }

    private static Object invoke(LuaState l, final Object obj, String methodName, int argCount) throws InvocationTargetException, IllegalAccessException, NoSuchMethodException {
        final Class<?> clazz;
        if (obj instanceof Class) {
            clazz = (Class<?>) obj;
        } else {
            clazz = obj.getClass();
        }
        for (Method method : clazz.getMethods()) {
            //方法名匹配
            if (method.getName().equals(methodName)
            ) {
                final Class<?>[] parameterTypes = method.getParameterTypes();
                if (method.isVarArgs()) {//变长参数方法
                    //变长方法最后一个参数以对象数组传入
                    //实际参数数量
                    final int realArgCount = parameterTypes.length - 1;
                    final Object[] args = new Object[parameterTypes.length];
                    if (ReflectionUtils.parseParameters(l, realArgCount, parameterTypes, args)) {
                        final int varArgc = argCount - realArgCount;
                        if (varArgc < 0) {
                            throw new IllegalArgumentException("Minimum number of arguments: " + realArgCount);
                        }
                        final Object[] varArgs = new Object[varArgc];
                        //最后一个参数为对象数组
                        args[realArgCount] = varArgs;
                        if (ReflectionUtils.parseVarArgs(l, realArgCount, varArgc, varArgs)) {
                            return method.invoke(obj, args);
                        }
                    }
                } else {
                    //参数数量相等
                    if (parameterTypes.length == argCount) {
                        final Object[] args = new Object[argCount];
                        if (ReflectionUtils.parseParameters(l, argCount, parameterTypes, args)) {
                            return method.invoke(obj, args);
                        }
                    }
                }
            }
        }

        throw new NoSuchMethodException("No found " + " method " + methodName);

    }


}
