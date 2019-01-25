package edu.scut.cs.hbdc.sysm.jni;

import edu.scut.cs.hbdc.sysm.Sysm;
import org.junit.Test;

public class ArchLoaderTest {
    @Test
    public void loadLib() throws Exception {
    }

    @Test
    public void testJarPath() throws Exception {
        new ArchLoader(Sysm.class).load(null);
    }

}