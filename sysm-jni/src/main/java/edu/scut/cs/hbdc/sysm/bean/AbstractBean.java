package edu.scut.cs.hbdc.sysm.bean;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import java.lang.reflect.Field;
import java.lang.reflect.Modifier;
import java.text.DecimalFormat;
import java.text.NumberFormat;
import java.util.HashMap;
import java.util.Map;

public class AbstractBean {
    private static final Logger log = LoggerFactory.getLogger(AbstractBean.class);
    private static final String DEFAULT_PERC_FORMAT = "#0.00";

    public void copyTo(AbstractBean copy) {
        if (copy == null) {
            throw new IllegalArgumentException("No copy object specified");
        }

        Class<?> clz = getClass(), copyClz = copy.getClass();
        if (clz != copyClz) {
            throw new IllegalArgumentException("The copy object's class is not same as this class");
        }

        Field[] props = clz.getDeclaredFields();

        for (Field prop: props) {
            int modifier = prop.getModifiers();
            if (!Modifier.isStatic(modifier)
                    && !Modifier.isFinal(modifier)) {
                boolean accessible = prop.isAccessible();
                try {
                    prop.setAccessible(true);
                    Object value = prop.get(this);
                    prop.set(copy, value);
                } catch (Exception e) {
                    log.error("Can't copy value", e);
                } finally {
                    prop.setAccessible(accessible);
                }
            }
        }
    }

    public Map<String, String> toMap() {
        Map<String, String> map = new HashMap<>();
        Class<?> clz = getClass();
        Field[] props = clz.getDeclaredFields();
        for (Field prop: props) {
            int modifier = prop.getModifiers();
            if (!Modifier.isStatic(modifier)
                    && !Modifier.isFinal(modifier)) {
                String name = prop.getName();
                boolean accessible = prop.isAccessible();
                try {
                    prop.setAccessible(true);
                    Object value = prop.get(this);
                    if (value != null) {
                        String strVal = String.valueOf(value);
                        if (prop.getType() == String.class) {
                            if (!"".equals(strVal))
                                map.put(name, strVal);
                        }

                        if (value instanceof Double
                                || value instanceof Float
                                || value instanceof Long
                                || value instanceof Integer) {
                            if (!"-1".equals(strVal)) {
                                map.put(name, strVal);
                            }
                        }
                    }
                }catch (Exception e) {
                    log.error("Can't put properties to map", e);
                } finally {
                    prop.setAccessible(accessible);
                }
            }
        }
        return map;
    }

    @Override
    public String toString() {
        return toMap().toString();
    }

    public static String format(double val) {
        return format(val, DEFAULT_PERC_FORMAT);
    }

    public static String format(double val, String format) {
        if (format == null || "".equals(format)){
            format = DEFAULT_PERC_FORMAT;
        }

        NumberFormat formatter = new DecimalFormat(format);
        return formatter.format(val * 100);
    }
}
