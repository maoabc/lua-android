package mao.commons.jlua.luajava;


import java.lang.reflect.Array;
import java.lang.reflect.Field;
import java.lang.reflect.InvocationTargetException;
import java.lang.reflect.Method;

import mao.commons.jlua.CJFunction;
import mao.commons.jlua.LuaException;
import mao.commons.jlua.LuaState;

/**
 * 通过反射执行的java对象的方法等
 */
public class ReflectionObject {

    private static final CJFunction indexFunc = new CJFunction() {
        @Override
        protected int call(LuaState luaState) throws Throwable {
            final Object obj = luaState.checkJavaObject(-2);

            final Class<?> cls = obj.getClass();
            if (cls.isArray()) {//数组相关
                int idx = luaState.checkInt(-1);
                if (idx < 0) {
                    final int length = Array.getLength(obj);
                    idx = length - idx;
                } else if (idx > 0) {
                    idx--;
                } else {
                    throw new LuaException("array index out");
                }
                final Object v = Array.get(obj, idx);
                pushJavaValue(luaState, v);
                return 1;
            }

            final String name = luaState.checkString(-1);

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
                    pushJavaValue(luaState, value);
                }
                return 1;
            }
            //查找同名方法
            if (ReflectionUtils.hasMethod(obj, name)) {//包含对象及方法名的闭包
                luaState.pushClosure(methodInvokeFunction, 1);
                return 1;
            }

            //找不到field或者method
            throw new LuaException(cls.getName() + " No such field or method: " + name);

        }
    };

    private static void pushJavaValue(LuaState luaState, Object value) {
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
            ReflectionObject.pushObject(luaState, value);
        }
    }

    private static final CJFunction newIndexFunc = new CJFunction() {
        @Override
        protected int call(LuaState luaState) throws Throwable {
            final Object arr = luaState.checkJavaObject(1);
            final Class<?> cls = arr.getClass();
            if (!cls.isArray()) {
                throw new LuaException("Not a array object");
            }
            int idx = luaState.checkInt(2);
            if (idx < 0) {
                final int length = Array.getLength(arr);
                idx = length - idx;
            } else if (idx > 0) {
                idx -= 1;
            } else {
                throw new LuaException("array index out");
            }
            if (cls == String[].class) {
                ((String[]) arr)[idx] = luaState.toLString(3);
            } else if (cls == boolean[].class) {
                ((boolean[]) arr)[idx] = luaState.toBoolean(3);
            } else if (cls == byte[].class) {
                ((byte[]) arr)[idx] = (byte) luaState.toInt32(3);
            } else if (cls == short[].class) {
                ((short[]) arr)[idx] = (short) luaState.toInt32(3);
            } else if (cls == char[].class) {
                ((char[]) arr)[idx] = (char) luaState.toInt32(3);
            } else if (cls == int[].class) {
                ((int[]) arr)[idx] = luaState.toInt32(3);
            } else if (cls == float[].class) {
                ((float[]) arr)[idx] = (float) luaState.toNumber(3);
            } else if (cls == long[].class) {
                ((long[]) arr)[idx] = luaState.toInt64(3);
            } else if (cls == double[].class) {
                ((double[]) arr)[idx] = luaState.toNumber(3);
            } else {
                Array.set(arr, idx, luaState.toJavaObject(3));
            }

            return 0;
        }
    };

    private static final CJFunction methodInvokeFunction = new CJFunction() {
        @Override
        protected int call(LuaState luaState) throws Throwable {
            final String methodName = luaState.checkString(LuaState.upValueIndex(1));
            //第一个参数为对象
            final Object ret = invoke(luaState, luaState.checkJavaObject(1), methodName, luaState.getTop() - 1);
            pushJavaValue(luaState, ret);
            return 1;
        }
    };


    public static void pushObject(LuaState l, Object obj) {

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
