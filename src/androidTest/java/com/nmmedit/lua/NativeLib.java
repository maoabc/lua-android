package com.nmmedit.lua;

public class NativeLib {

    // Used to load the 'lua' library on application startup.
    static {
        System.loadLibrary("jlua");
    }

    /**
     * A native method that is implemented by the 'lua' native library,
     * which is packaged with this application.
     */
    public static native String stringFromJNI();
}