package edu.scut.cs.hbdc.sysm.bean;

import org.junit.Test;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import java.lang.reflect.Field;

import static org.junit.Assert.*;

public class AbstractBeanTest {
    private static final Logger log = LoggerFactory.getLogger(AbstractBean.class);
    @Test
    public void copyToMap() throws Exception {
        /*Memory*/
        logInfo("------------Test Memory------------");
        Memory memory = new Memory(), memory1 = new Memory();
        setProperty(memory, "total", 4096L);
        memory.copyTo(memory1);
        assertEquals(4096L, memory1.getTotal());
        logInfo(memory);
        logInfo(memory1.toMap());

        /*Swap*/
        logInfo("------------Test Swap------------");
        Swap swap = new Swap(), swap1 = new Swap();
        setProperty(swap, "free", 2048L);
        swap.copyTo(swap1);
        assertEquals(2048L, swap1.getFree());
        logInfo(swap);
        logInfo(swap1.toMap());

        /*Cpu*/
        logInfo("------------Test Cpu------------");
        Cpu cpu = new Cpu(), cpu1 = new Cpu();
        setProperty(cpu, "user", 10L);
        cpu.copyTo(cpu1);
        assertEquals(10L, cpu1.getUser());
        logInfo(cpu);
        logInfo(cpu1.toMap());
    }

    private void setProperty(AbstractBean device, String propertyName, Object value) throws Exception{
        Field property = device.getClass().getDeclaredField(propertyName);
        Class<?> type = property.getType();
        Class<?> mytype = Long.TYPE;
        boolean accessible = property.isAccessible();
        property.setAccessible(true);

        if (type.isAssignableFrom(Double.TYPE) && value instanceof Double) {
            property.setDouble(device, (Double) value);
        }

        if (type.isAssignableFrom(Long.TYPE) && value instanceof Long) {
            property.setLong(device, (Long) value);
        }

        property.setAccessible(false);
    }

    private void logInfo(Object obj) {
        log.info(obj.toString());
    }

}