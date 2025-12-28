
# Lua-Android

一个基于官方 Lua 解释器 + libffi 的高性能 Android Lua 绑定方案。

## 主要原因

早期使用 luaj 将 App 的部分操作导出到 Lua 以支持自定义，但 luaj 早已停止维护。后来决定切换到官方 Lua 版本。

研究了 luajava 及其各种衍生项目后发现：
- 扩展性差：高效模块几乎都必须在 native 层通过 JNIEnv 手动调用 Java，编写麻烦且容易出错。
- 现有方案性能不足：
    - luajava 基于反射，速度慢。
    - JavaFunction 需要在 Lua 层通过 userdata + 元表绑定，调用链长。
- 想要高性能就无法将 Lua 绑定做成独立的module，只能把 native 代码写在主项目中。

核心改进是**使用 libffi 动态绑定 Java 函数对象与 C 函数**：
- 每个 Java 函数对象都会动态生成对应的 C 函数闭包(里面使用JNIEnv调用java)。将这个 C 函数 push 到 Lua 栈，即相当于 push 一个可直接调用的 Java 函数。
- 所有 Lua → Java 的实际调用仅在一处集中使用 JNIEnv，其余 lua_State 操作全部导出为 Java native 方法。这样所有 Lua 模块都可以在纯 Java 层实现，开发简单、性能更高（尤其是可以利用 `@FastNative` 和 `@CriticalNative` 优化）。

## 使用方式

1. 将本项目 clone 到你的工程目录下。
2. 作为普通的 Android Library module 添加到主 app 的依赖中。
3. 在根目录 `gradle.properties` 或模块的 `build.gradle` 中，将相关版本变量替换为你项目实际使用的版本（ndkVersion、compileSdk等）。

## 一些例子

### Lua 代码调用 Java 实现的函数

```java
try (final LuaState l = LuaState.create()) {
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

    l.loadBuffer("return jadd(3,4)");
    l.call(0, 1);

final int i = l.checkInt(-1);
    System.out.println("result " + i);  // 输出 7
}
```

### Java 对象传递给 Lua 并添加自定义方法
```java
try (final LuaState state = LuaState.create()) {
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
```
### luajava模块相关
项目也提供了类似 luajava 的反射式绑定（通过 ReflectionObject.pushObject 等），方便快速让 Lua 直接操作 Java 对象，但性能稍低。推荐在性能敏感场景使用上面手动注册 CJFunction 的方式，更快且更灵活。

## License
- lua和libffi等第三方库有各自的licence
- 其余基于Apache2.0
