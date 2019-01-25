package edu.scut.cs.hbdc.sysm.hello;

import org.junit.Test;

public class HelloTest {
    @Test
    public void hello() throws Exception {
        System.load("/home/wzt/IdeaProjects/hbdc/hbdc-sysm-native/cmake-build-debug/libhbdc_sysm_native.so");
        Hello.hello();
    }

    @Test
    public void testSbd() throws Exception {
        StringBuilder sbd = new StringBuilder();
        System.out.println(sbd);
        changeSbd(sbd);
        System.out.println(sbd);
    }

    public void changeSbd(StringBuilder sbd) {
        sbd.append("test");
    }

}