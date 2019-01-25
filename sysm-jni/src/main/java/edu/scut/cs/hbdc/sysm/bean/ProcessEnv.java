package edu.scut.cs.hbdc.sysm.bean;

import edu.scut.cs.hbdc.sysm.exception.SysmException;
import edu.scut.cs.hbdc.sysm.Sysm;

import java.util.Map;

public class ProcessEnv {

    private ProcessEnv() {}

    public static native Map<String, String> getAll(Sysm sysm, long pid)
        throws SysmException;

    public static native String getValue(Sysm sysm, long pid, String key)
        throws SysmException;
}
