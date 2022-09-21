package mao.commons.jlua;


import org.junit.Test;

import java.io.File;
import java.io.FileDescriptor;
import java.io.FileOutputStream;
import java.io.RandomAccessFile;
import java.nio.channels.FileChannel;
import java.nio.file.Paths;

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
                ",ddd=function() return \"ttyt\" end\n" +
                ",fff=function() end\n" +
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


    private CJFunction doFileFunc = new CJFunction() {
        @Override
        protected int call(LuaState luaState) throws Throwable {
            final String s = luaState.checkLString(-1);
            return 0;
        }
    };

    @Test
    public void testDoFile() {
        final LuaState luaState = LuaState.create();
        luaState.pushFunction(doFileFunc);
        luaState.setGlobal("dofile");
        luaState.getGlobal("dofile");
        final int type = luaState.type(-1);
        luaState.loadBuffer("local a=dofile(\"dkdlkj\")");
        luaState.pcall(0, 0, 0);
        System.out.println();
    }

    @Test
    public void testCustomLoad() {
        final LuaState luaState = LuaState.create();
        LuaAndroid.setup(luaState, new ResourceFinder() {
            @Override
            public byte[] findResource(String filename) {
                return "return 6".getBytes();
            }
        });
        luaState.loadBuffer("return loadfile(\"my\")");
        luaState.pcall(0, 1, 0);
        final int type = luaState.type(-1);
        final String s = luaState.toLString(-1);
        final boolean integer = luaState.isInteger(-1);

        System.out.println();


    }

    @Test
    public void testIterTable() {
        final LuaState luaState = LuaState.create();
        luaState.loadBuffer("return {1,3}");
        final int pcall = luaState.pcall(0, 1, 0);
//        final int type3 = luaState.type(-1);
//        final String s = luaState.checkLString(-1);
        luaState.pushNil();
        while (luaState.next(-2)) {
            final int type1 = luaState.type(-2);
            final int type2 = luaState.type(-1);

            luaState.pop(1);
        }
        final int type = luaState.type(-1);
        final boolean integer = luaState.isInteger(-1);

        System.out.println();


    }
}