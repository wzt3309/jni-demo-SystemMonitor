package edu.scut.cs.hbdc.sysm.bean;

import edu.scut.cs.hbdc.sysm.exception.SysmException;
import edu.scut.cs.hbdc.sysm.Sysm;

import java.io.Serializable;

public class Swap extends AbstractBean implements Serializable{

    private static final long serialVersionUID = 5218483109795811484L;
    private long total;
    private long used;
    private long free;
    private long pageIn;
    private long pageOut;

    public Swap() {
    }

    public native void gather(Sysm sysm) throws SysmException;

    public static Swap fetch(Sysm sysm) throws SysmException {
        Swap swap = new Swap();
        swap.gather(sysm);
        return swap;
    }
    /**
     * Get the Total system swap
     * @return Total system swap
     */
    public long getTotal() {
        return total;
    }

    /**
     * Get the Total used system swap
     * @return Total used system swap
     */
    public long getUsed() {
        return used;
    }

    /**
     * Get the Total free system swap
     * @return Total free system swap
     */
    public long getFree() {
        return free;
    }

    /**
     * Get the Pages in
     * /proc/vmstat ps
     * @return Pages in
     */
    public long getPageIn() {
        return pageIn;
    }

    /**
     * Get the Pages out
     * /proc/vmstat pswpout
     * @return Pages out
     */
    public long getPageOut() {
        return pageOut;
    }

    @Override
    public String toString() {
        return "Swap: " +
                (this.total / 1024) + "K total, " +
                (this.used / 1024) + "K used, " +
                (this.free/ 1024) + "K free";
    }
}
