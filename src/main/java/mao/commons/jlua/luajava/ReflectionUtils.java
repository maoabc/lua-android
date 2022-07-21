package mao.commons.jlua.luajava;

import java.lang.reflect.Field;
import java.lang.reflect.Method;

public class ReflectionUtils {

    public static Field getField(Object objOrClass, String fieldName) {
        final Class<?> clazz;
        if (objOrClass instanceof Class) {
            clazz = (Class<?>) objOrClass;
        } else {
            clazz = objOrClass.getClass();
        }
        try {
            return clazz.getField(fieldName);
        } catch (final NoSuchFieldException ignored) {
        }
        return null;

    }

    public static Method getMethod(Object objOrClass, String methodName) {
        final Class<?> clazz;
        if (objOrClass instanceof Class) {
            clazz = (Class<?>) objOrClass;
        } else {
            clazz = objOrClass.getClass();
        }
        for (Method method : clazz.getMethods()) {
            if (method.getName().equals(methodName)) {
                return method;
            }
        }

        return null;

    }

    public static Object findAndReadField(Object objOrClass, String fieldName) {
        final Object obj;
        final Class<?> clazz;
        if (objOrClass instanceof Class) {
            clazz = (Class<?>) objOrClass;
            obj = null;
        } else {
            clazz = objOrClass.getClass();
            obj = objOrClass;
        }
        try {
            final Field field = clazz.getField(fieldName);
//            if (!Modifier.isPublic(field.getModifiers())) {
//                field.setAccessible(true);
//            }
            return field.get(obj);
        } catch (final NoSuchFieldException | IllegalAccessException ignored) {
        }
        return null;

    }
}
