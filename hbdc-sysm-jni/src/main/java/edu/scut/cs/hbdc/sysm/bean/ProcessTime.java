package edu.scut.cs.hbdc.sysm.bean;

import edu.scut.cs.hbdc.sysm.exception.SysmException;
import edu.scut.cs.hbdc.sysm.Sysm;

import java.io.Serializable;

public class ProcessTime extends AbstractBean implements Serializable {

    private static final long serialVersionUID = -3725565335469224312L;
    private long startTime;
    private long user;
    private long sys;
    private long total;

    public ProcessTime() {
    }

    public native void gather(Sysm sysm, long pid) throws SysmException;

    public static ProcessTime fetch(Sysm sysm, long pid) throws SysmException {
        ProcessTime processTime = new ProcessTime();
        processTime.gather(sysm, pid);
        return processTime;
    }

    /**
     * Get the Time process was started in seconds
     * @return Time process was started in seconds
     */
    public long getStartTime() {
        return startTime;
    }

    /**
     * Get the Process cpu user time
     * @return Process cpu user time
     */
    public long getUser() {
        return user;
    }

    /**
     * Get the Process cpu kernel time
     * @return Process cpu kernel time
     */
    public long getSys() {
        return sys;
    }

    /**
     * Get the Process cpu time (sum of User and Sys)
     * @return Process cpu time (sum of User and Sys)
     */
    public long getTotal() {
        return total;
    }
}
