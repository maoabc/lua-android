package mao.commons.jlua;

/**
 * 保留一组c函数指针,用于高效处理异常等
 */
public class FunctionUtils {

    public static native long traceback();
}
