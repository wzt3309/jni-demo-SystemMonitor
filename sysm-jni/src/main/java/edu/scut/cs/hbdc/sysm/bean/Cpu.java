package edu.scut.cs.hbdc.sysm.bean;

import edu.scut.cs.hbdc.sysm.exception.SysmException;
import edu.scut.cs.hbdc.sysm.Sysm;

import java.io.Serializable;

public class Cpu extends AbstractBean implements Serializable {

    private static final long serialVersionUID = 5216973339055138142L;
    private long user;
    private long sys;
    private long nice;
    private long idle;
    private long wait;
    private long hi;
    private long si;
    private long steal;
    private long total;

    public Cpu() {
    }

    public native void gather(Sysm sysm) throws SysmException;

    public static Cpu fetch(Sysm sysm) throws SysmException {
        Cpu cpu = new Cpu();
        cpu.gather(sysm);
        return cpu;
    }

    /**
     * Get the Total system cpu user time
     * <p>CPU time spent in user space</p>
     * @return Total system cpu user time
     */
    public long getUser() {
        return user;
    }

    /**
     * Get the Total system cpu kernel time
     * <p>CPU time spent in kernel space</p>
     * @return Total system cpu kernel time
     */
    public long getSys() {
        return sys;
    }

    /**
     * Get the Total system cpu nice time
     * <p>CPU time spent on low priority processes</p>
     * @return Total system cpu nice time
     */
    public long getNice() {
        return nice;
    }

    /**
     * Get the Total system cpu idle time
     * <p>CPU time spent idle</p>
     * @return Total system cpu idle time
     */
    public long getIdle() {
        return idle;
    }

    /**
     * Get the Total system cpu io wait time
     * @return CPU time spent in wait (on disk)
     */
    public long getWait() {
        return wait;
    }

    /**
     * Get the Total system cpu time servicing/handling hardware interrupts
     * @return Total system cpu time servicing/handling hardware interrupts
     */
    public long getHi() {
        return hi;
    }

    /**
     * Get the Total system cpu time servicing/handling software interrupts
     * @return Total system cpu time servicing/handling software interrupts
     */
    public long getSi() {
        return si;
    }

    /**
     * Get the Total system cpu involuntary wait time
     * @return Total system cpu involuntary wait time
     */
    public long getSteal() {
        return steal;
    }

    /**
     * Get the Total system cpu time
     * @return Total system cpu time
     */
    public long getTotal() {
        return total;
    }
}
