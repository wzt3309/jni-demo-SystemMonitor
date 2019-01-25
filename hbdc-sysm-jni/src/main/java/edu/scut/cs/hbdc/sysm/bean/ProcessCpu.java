package edu.scut.cs.hbdc.sysm.bean;

import edu.scut.cs.hbdc.sysm.exception.SysmException;
import edu.scut.cs.hbdc.sysm.Sysm;

import java.io.Serializable;

public class ProcessCpu extends AbstractBean implements Serializable {

    private static final long serialVersionUID = 1870673427995892017L;
    private double percent;
    private long lastTime;
    private long startTime;
    private long user;
    private long sys;
    private long total;

    public ProcessCpu() {
    }


    public native void gather(Sysm sysm, long pid) throws SysmException;

    public static ProcessCpu fetch(Sysm sysm, long pid) throws SysmException {
        ProcessCpu processCpu = new ProcessCpu();
        processCpu.gather(sysm, pid);
        return processCpu;
    }
    /**
     * Get the Process cpu usage
     * @return Process cpu usage
     */
    public double getPercent() {
        return percent;
    }

    protected void setPercent(double percent) {
        this.percent = percent;
    }
    /**
     * Get the last_time
     * The last time monitoring the use of cpu by process
     * @return last_time
     */
    public long getLastTime() {
        return lastTime;
    }

    protected void setLastTime(long lastTime) {
        this.lastTime = lastTime;
    }

    /**
     * Get the Time process was started in seconds
     * @return Time process was started in seconds
     */
    public long getStartTime() {
        return startTime;
    }

    protected void setStartTime(long startTime) {
        this.startTime = startTime;
    }

    /**
     * Get the Process cpu user time
     * @return Process cpu user time
     */
    public long getUser() {
        return user;
    }

    protected void setUser(long user) {
        this.user = user;
    }

    /**
     * Get the Process cpu kernel time
     * @return Process cpu kernel time
     */
    public long getSys() {
        return sys;
    }

    protected void setSys(long sys) {
        this.sys = sys;
    }

    /**
     * Get the Process cpu time (sum of User and Sys)
     * @return Process cpu time (sum of User and Sys)
     */
    public long getTotal() {
        return total;
    }

    protected void setTotal(long total) {
        this.total = total;
    }
}
