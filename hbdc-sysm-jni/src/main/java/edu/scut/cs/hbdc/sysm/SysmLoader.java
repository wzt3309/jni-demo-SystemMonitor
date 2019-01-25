package edu.scut.cs.hbdc.sysm;

import edu.scut.cs.hbdc.sysm.jni.ArchLoader;
import edu.scut.cs.hbdc.sysm.jni.ArchNotSupportedException;

public class SysmLoader extends ArchLoader {

    private static final String PROP_SYSM_JAR_NAME = "sysm.jar.name";
    private static final String PROP_SYSM_LIB_NAME = "sysm.lib.name";

    public SysmLoader() {
        super(Sysm.class);
    }

    public String getDefaultLibName() throws ArchNotSupportedException {
        return System.getProperty(PROP_SYSM_LIB_NAME,
                "hbdc" + getArchLibName());
    }

    public String getJarName() {
        return System.getProperty(PROP_SYSM_JAR_NAME,
                super.getJarName());
    }
}
