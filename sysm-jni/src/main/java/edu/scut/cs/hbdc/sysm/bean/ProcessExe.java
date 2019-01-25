package edu.scut.cs.hbdc.sysm.bean;

import edu.scut.cs.hbdc.sysm.exception.SysmException;
import edu.scut.cs.hbdc.sysm.Sysm;

import java.io.Serializable;

public class ProcessExe extends AbstractBean implements Serializable {

    private static final long serialVersionUID = -8037854520893715933L;
    private String name;
    private String cwd;

    public ProcessExe() {
    }

    public native void gather(Sysm sysm, long pid) throws SysmException;

    public static ProcessExe fetch(Sysm sysm, long pid) throws SysmException{
        ProcessExe processExe = new ProcessExe();
        processExe.gather(sysm, pid);
        return processExe;
    }

    /**
     * Get the Name of process executable
     * In linux <code>ls -l /proc/${pid}/exe</code>
     * @return Name of process executable
     */
    public String getName() {
        return name;
    }

    /**
     * Get the Name of process current working directory
     * In linux <code>ls -l /proc/${pid}/cwd</code>
     * @return Name of process current working directory
     */
    public String getCwd() {
        return cwd;
    }
}
