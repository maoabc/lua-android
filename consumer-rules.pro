-keepclassmembers class mao.commons.jlua.* {
    native <methods>;
}

-keep class mao.commons.jlua.LuaException
-keepclassmembers class mao.commons.jlua.LuaException {
    <init>(...);
}
-keepclassmembers class mao.commons.jlua.LuaJNI {
    public static final <fields>;
}

-keepclassmembers class mao.commons.jlua.JFunction {
    *;
}
-keepclassmembers class mao.commons.jlua.ResourceFinder {
    *;
}

