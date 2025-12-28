package mao.commons.jlua;


import org.junit.Test;

public class LuaStateTest {

    @Test
    public void testLuaCallJava() {
        try (
                final LuaState l = LuaState.create();
        ) {
            l.pushCFunction(UtilFunctions.traceback());
            //测试代码，函数写成局部变量，不过马上整个环境就失效，所以没生命周期问题
            final CJFunction addFunc = new CJFunction() {
                @Override
                protected int call(LuaState luaState) {
                    final int i1 = luaState.checkInt(-1);
                    final int i2 = luaState.checkInt(-2);
                    System.out.println("java impl add function " + i1 + "  " + i2);
                    luaState.pushInt32(i1 + i2);
                    return 1;
                }
            };
            l.pushFunction(addFunc);
            l.setGlobal("jadd");
            //lua代码调用jadd
            l.loadBuffer("return jadd(3,4)");
            l.call(0, 1);
            //取得lua代码执行结果
            final int i = l.checkInt(-1);
            System.out.println("result " + i);

        }
    }

    public static void asyncTask(LuaCallbackContext context) {
        if (!context.l.isFunction(-1)) {
            throw new IllegalStateException("Not a function");
        }
        new Thread(new Runnable() {
            @Override
            public void run() {
                //耗时任务执行
                //回调lua
                context.l.pushString("java task finish");
                context.l.call(1, 1);

            }
        }).start();

    }

    @Test
    public void testCallback() throws Exception {
        try (
                LuaState l = LuaState.create();
        ) {
            l.pushCFunction(UtilFunctions.traceback());

            final CJFunction asyncFunc = new CJFunction() {
                @Override
                protected int call(LuaState luaState) {
                    final LuaCallbackContext context = new LuaCallbackContext(luaState);
                    asyncTask(context);
                    return 0;
                }
            };
            l.pushFunction(asyncFunc);
            l.setGlobal("asyncTask");

            l.loadBuffer(
                    "local function task(n) \n" +
                            "print(n)\n" +
                            "end\n" +
                            "\n" +
                            "asyncTask(task)" +
                            "\n");
            l.call(0, 0);

            Thread.sleep(3000);


        }
    }

    @Test
    public void create() {
        final LuaState state = LuaState.create();
        final CJFunction function = new CJFunction() {
            @Override
            protected int call(LuaState luaState) {
                final int i1 = luaState.checkInt(-1);
                final int i2 = luaState.checkInt(-2);
                luaState.pushInt32(i1 + i2);
                return 1;
            }
        };
        state.pushClosure(function, 0);
        state.pushInt32(4);
        state.pushInt32(5);
        state.call(2, 1);
        final int i = state.toInt32(1);
        state.setGlobal("myv");
        state.getGlobal("myv");
        final int i1 = state.toInt32(-1);
        state.loadBuffer("function abc(a) return a*a end");
        state.call(0, 0);
        state.getGlobal("abc");
        state.pushInt32(4);
        state.call(1, 1);
        final int i2 = state.toInt32(-1);

        System.out.println(i);
    }

    @Test
    public void testJavaObject() {
        try (final LuaState state = LuaState.create()) {
            state.pushCFunction(UtilFunctions.traceback());

            Object obj = Class.class;
            state.pushJavaObject(obj);


            final CJFunction tostringFunc = new CJFunction() {
                @Override
                protected int call(LuaState luaState) {
                    final Object o = luaState.checkJavaObject(LuaState.upValueIndex(1));
                    luaState.pushString("lua tostring: " + o);
                    return 1;
                }
            };
            final CJFunction indexFunc = new CJFunction() {
                @Override
                protected int call(LuaState luaState) {
                    final boolean javaObject = luaState.isJavaObject(-2);
                    final Object object = luaState.toJavaObject(-2);
                    final boolean string = luaState.isString(-1);
                    final String methodName = luaState.toLString(-1);
                    if ("toString".equals(methodName)) {
                        luaState.pushClosure(tostringFunc, 2);
                    }
                    return 1;
                }
            };
            state.getMetatable(-1);

            state.pushString("__index");
            state.pushClosure(indexFunc, 0);
            state.rawSet(-3);
            state.pop(1);

            state.setGlobal("mys");

            state.loadBuffer("return mys.toString()");
            state.call(0, 1);

            System.out.println("result " + state.checkString(-1));
        }

    }
}