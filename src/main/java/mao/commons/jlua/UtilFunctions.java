package mao.commons.jlua;

import dalvik.annotation.optimization.FastNative;

/**
 * 保留一组c函数指针,用于高效处理异常等
 */
public class UtilFunctions {

    @FastNative
    public static native long traceback();

    @FastNative
    public static native long emptyFunc();
}
