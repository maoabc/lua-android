package mao.commons.jlua.luajava;

import java.lang.reflect.Constructor;
import java.lang.reflect.Field;
import java.lang.reflect.InvocationTargetException;
import java.lang.reflect.Method;

import mao.commons.jlua.LuaState;

public class ReflectionUtils {

    public static Field getField(Object obj, String fieldName) {
        final Class<?> clazz;
        if (obj instanceof Class) {
            clazz = (Class<?>) obj;
        } else {
            clazz = obj.getClass();
        }
        try {
            return clazz.getField(fieldName);
        } catch (final NoSuchFieldException ignored) {
        }
        return null;

    }

    //简单查找方法名判断是否包含同名方法
    public static boolean hasMethod(Object obj, String methodName) {
        final Class<?> clazz;
        if (obj instanceof Class) {
            clazz = (Class<?>) obj;
        } else {
            clazz = obj.getClass();
        }
        for (Method method : clazz.getMethods()) {
            if (method.getName().equals(methodName)) {
                return true;
            }
        }
        return false;
    }

    static boolean parseVarArgs(LuaState l, int argIdx, int varArgc, Object[] varArgs) {
        for (int i = 0; i < varArgc; i++) {
            final int idx = argIdx + 1 + i + 1;
            final int luaType = l.type(idx);
            if (luaType == LuaState.LUA_TSTRING) {
                varArgs[i] = l.toLString(idx);
            } else if (luaType == LuaState.LUA_TNUMBER) {
                if (l.isInteger(idx)) {
                    varArgs[i] = l.toInt64(idx);
                } else {//double
                    varArgs[i] = l.toNumber(idx);
                }
            } else if (luaType == LuaState.LUA_TBOOLEAN) {
                varArgs[i] = l.toBoolean(idx);
            } else if (luaType == LuaState.LUA_TNIL) {
                varArgs[i] = null;
            } else if (l.isJavaObject(idx)) {
                varArgs[i] = l.checkJavaObject(idx);
            } else {
                throw new IllegalArgumentException("Lua type " + l.typeName(luaType));
                //lua内部数据,没法传递给java
            }
        }
        return true;
    }

    static boolean parseParameters(LuaState l,
                                   int parameterCount,
                                   final Class<?>[] parameterTypes, Object[] outArgs) {
        for (int i = 0; i < parameterCount; i++) {
            final Class<?> type = parameterTypes[i];
            if (!matchType(l, type, outArgs, i)) {
                return false;
            }
        }
        return true;
    }

    static boolean matchType(LuaState l, Class<?> parameterType, Object[] outArgs, int parameterIdx) {
        final int idx = parameterIdx + 1 + 1;//lua从1开始,同时第一个参数为对象本身需要排除
        final int luaType = l.type(idx);
        if (parameterType.isPrimitive()) {
            if (parameterType == Boolean.TYPE && luaType == LuaState.LUA_TBOOLEAN) {//是否为boolean
                outArgs[parameterIdx] = l.toBoolean(idx);
                return true;
            }
            if (luaType == LuaState.LUA_TNUMBER) {
                if (parameterType == Byte.TYPE) {
                    outArgs[parameterIdx] = (byte) l.toInt64(idx);
                } else if (parameterType == Short.TYPE) {
                    outArgs[parameterIdx] = (short) l.toInt64(idx);
                } else if (parameterType == Character.TYPE) {
                    outArgs[parameterIdx] = (char) l.toInt64(idx);
                } else if (parameterType == Integer.TYPE) {
                    outArgs[parameterIdx] = (int) l.toInt32(idx);
                } else if (parameterType == Float.TYPE) {
                    outArgs[parameterIdx] = (float) l.toNumber(idx);
                } else if (parameterType == Long.TYPE) {
                    outArgs[parameterIdx] = (long) l.toInt64(idx);
                } else if (parameterType == Double.TYPE) {
                    outArgs[parameterIdx] = (double) l.toNumber(idx);
                }
                return true;
            }
            return false;
        }
        if (parameterType == String.class && luaType == LuaState.LUA_TSTRING) {//字符串
            outArgs[parameterIdx] = l.toLString(idx);
            return true;
        }
        if (l.isJavaObject(idx)) {//非空对象
            final Object object = l.checkJavaObject(idx);
            if (parameterType.isAssignableFrom(object.getClass())) {
                outArgs[parameterIdx] = object;
                return true;
            } else {
                return false;
            }
        }
        if (luaType == LuaState.LUA_TNIL) {
            outArgs[parameterIdx] = null;
            return true;
        }
        return false;
    }


    static Object invokeConstructor(LuaState l, final Class<?> clazz, int argCount) throws IllegalAccessException, InstantiationException, NoSuchMethodException, InvocationTargetException {
        for (Constructor<?> constructor : clazz.getConstructors()) {
            final Class<?>[] parameterTypes = constructor.getParameterTypes();
            if (constructor.isVarArgs()) {//变长参数方法
                //变长方法最后一个参数以对象数组传入
                //实际参数数量
                final int realArgCount = parameterTypes.length - 1;
                final Object[] args = new Object[parameterTypes.length];
                if (parseParameters(l, realArgCount, parameterTypes, args)) {
                    final int varArgc = argCount - realArgCount;
                    if (varArgc < 0) {
                        throw new IllegalArgumentException("Minimum number of arguments: " + realArgCount);
                    }
                    final Object[] varArgs = new Object[varArgc];
                    //最后一个参数为对象数组
                    args[realArgCount] = varArgs;
                    if (parseVarArgs(l, realArgCount, varArgc, varArgs)) {
                        return constructor.newInstance(args);
                    }
                }
            } else {
                //参数数量相等
                if (parameterTypes.length == argCount) {
                    final Object[] args = new Object[argCount];
                    if (parseParameters(l, argCount, parameterTypes, args)) {
                        return constructor.newInstance(args);
                    }
                }
            }
        }

        throw new NoSuchMethodException("No found " + " constructor ");

    }
}
