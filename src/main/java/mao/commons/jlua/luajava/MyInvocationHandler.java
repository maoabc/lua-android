package mao.commons.jlua.luajava;

import java.lang.reflect.InvocationHandler;
import java.lang.reflect.Method;

import mao.commons.jlua.LuaCallbackContext;
import mao.commons.jlua.LuaException;
import mao.commons.jlua.LuaState;

public class MyInvocationHandler implements InvocationHandler {

    private final LuaCallbackContext context;

    public MyInvocationHandler(LuaCallbackContext context) {
        this.context = context;
    }

    @Override
    public Object invoke(Object proxy, Method method, Object[] args) throws Throwable {
        final LuaState l = context.l;
        if (l.getField(-1, method.getName()) != LuaState.LUA_TFUNCTION) {
            throw new LuaException("No implementation method " + method.getName());
        }
        int nargs;
        if (method.isVarArgs()) {
            //除去最后的变长数组
            nargs = args.length - 1;
            parseArgs(args, nargs, l);

            //变长参数
            final Object[] varargs = (Object[]) args[args.length - 1];
            parseArgs(varargs, varargs.length, l);
            nargs += varargs.length;
        } else {
            nargs = args.length;
            parseArgs(args, nargs, l);
        }

        final Class<?> returnType = method.getReturnType();
        if (returnType == Void.class || returnType == void.class) {
            //调用lua函数
            if (l.pcall(nargs, 0, 1) != 0) {
                String msg = "";
                if (l.isString(-1)) {
                    msg = l.toLString(-1);
                }
                throw new LuaException("Lua pcall error: " + msg);
            }
            return null;
        } else {
            //调用lua函数
            if (l.pcall(nargs, 1, 1) != 0) {
                String msg = "";
                if (l.isString(-1)) {
                    msg = l.toLString(-1);
                }
                throw new LuaException("Lua pcall error: " + msg);
            }
            final Object ret;
            if (returnType == int.class || returnType == Integer.class) {
                ret = l.toInt32(-1);
            } else if (returnType == String.class) {
                ret = l.toLString(-1);
            } else if (returnType == boolean.class || returnType == Boolean.class) {
                ret = l.toBoolean(-1);
            } else if (returnType == byte.class || returnType == Byte.class) {
                ret = (byte) l.toInt64(-1);
            } else if (returnType == short.class || returnType == Short.class) {
                ret = (short) l.toInt64(-1);
            } else if (returnType == char.class || returnType == Character.class) {
                ret = (char) l.toInt64(-1);
            } else if (returnType == float.class || returnType == Float.class) {
                ret = (float) l.toNumber(-1);
            } else if (returnType == double.class || returnType == Double.class) {
                ret = l.toNumber(-1);
            } else if (returnType == long.class || returnType == Long.class) {
                ret = l.toInt64(-1);
            } else {
                if (l.isNil(-1)) {
                    ret = null;
                } else {
                    ret = l.toJavaObject(-1);
                }
            }
            l.pop(1);
            return ret;
        }

    }


    private static void parseArgs(Object[] args, int argc, LuaState l) {
        for (int i = 0; i < argc; i++) {
            Object arg = args[i];
            //null对应到lua的nil
            if (arg == null) {
                l.pushNil();
                continue;
            }
            final Class<?> argCls = arg.getClass();
            if (argCls.isPrimitive()) {
                if (argCls == Boolean.class) {
                    l.pushBoolean((Boolean) arg);
                } else if (argCls == Byte.class) {
                    l.pushInt64((Byte) arg);
                } else if (argCls == Short.class) {
                    l.pushInt64((Short) arg);
                } else if (argCls == Character.class) {
                    l.pushInt64((Character) arg);
                } else if (argCls == Integer.class) {
                    l.pushInt64((Integer) arg);
                } else if (argCls == Long.class) {
                    l.pushInt64((Long) arg);
                } else if (argCls == Float.class) {
                    l.pushNumber((Float) arg);
                } else if (argCls == Double.class) {
                    l.pushNumber((Double) arg);
                }
            } else {
                //对象类型
                //字符串特殊处理,转换为lua的字符串
                if (argCls == String.class) {
                    l.pushString((String) arg);
                } else {
                    ReflectionObject.registerObject(l, arg);
                }
            }
        }
    }
}
