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
    }
}