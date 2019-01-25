package edu.scut.cs.hbdc.sysm.jni;

import edu.scut.cs.hbdc.sysm.Sysm;
import org.junit.Test;

import java.nio.file.Path;

public class LibFinderTest {
    String libName = "junit-4.12.pom";
    String jarName = "junit.jar";
    Class clz = Sysm.class;
    ClassLoader loader = clz.getClassLoader();
    @Test
    public void find() throws Exception {
    }

    @Test
    public void findBesideJar() throws Exception {
        StringBuilder sbd = new StringBuilder(jarName);
        Path path = LibFinder
                        .findBesideJar(libName,
                                       loader,
                                       "org/junit",
                                       sbd);
        jarName = sbd.toString();
        System.out.println(jarName);
        System.out.println(path);
    }

    @Test
    public void findInClasspath() throws Exception {
        Path path = LibFinder
                .findInClasspath("test.so",
                                  loader,
                                 "edu/scut");
        System.out.println(path);
    }

    @Test
    public void findBesideClasspath() throws Exception {
        Path path = LibFinder
                .findBesideClasspath(
                        "test.so",
                         loader);
        System.out.println(path);
    }
}