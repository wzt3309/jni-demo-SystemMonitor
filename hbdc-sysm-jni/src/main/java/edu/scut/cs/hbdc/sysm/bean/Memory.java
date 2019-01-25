package edu.scut.cs.hbdc.sysm.bean;

import edu.scut.cs.hbdc.sysm.exception.SysmException;
import edu.scut.cs.hbdc.sysm.Sysm;

import java.io.Serializable;

/**
 * A class hold information of System Memory
 */
public class Memory extends AbstractBean implements Serializable{

    private static final long serialVersionUID = -1415576389011166337L;
    private long total;
    private long mtrr;
    private long used;
    private long free;              //total - used
    private long actualUsed;        //used - cache/buffers like "<cmd>free</cmd>" second line
    private long actualFree;        //free + cache/buffers like "<cmd>free</cmd>" second line
    private double usedPercent;
    private double freePercent;

    public Memory() {
    }

    /**
     * The c struct of sysm carry the Memory's fields' jni id
     * the method's jni implementation will set field of a memory
     * obj from system
     * @param sysm
     * @throws SysmException
     */
    public native void gather(Sysm sysm) throws SysmException;

    public static Memory fetch(Sysm sysm) throws SysmException {
        Memory memory = new Memory();
        memory.gather(sysm);
        return memory;
    }

    /**
     * Get the Total installed memory
     * see /proc/meminfo MemTotal
     * @return Total installed memory
     */
    public long getTotal() {
        return total;
    }

    /**
     * Get Memory Type Range Registers(MB)
     * @return MTRR
     */
    public long getMtrr() {
        return mtrr;
    }

    /**
     * Get the Total Used memory including buffers and cached
     * total - free
     * A buffer is something that has yet to be “written” to disk
     * A cache is something that has been “read” from the disk and stored for later use
     * @return Total Used memory
     */
    public long getUsed() {
        return used;
    }

    /**
     * Get the Total Free memory not including buffers and cached
     * see /proc/meminfo MemFree
     * @return Total Free memory
     */
    public long getFree() {
        return free;
    }

    /**
     * Get the Actual Used memory
     * <p>used - cache/buffers
     * @return Actual Used memory
     */
    public long getActualUsed() {
        return actualUsed;
    }

    /**
     * Get the Actual Free memory
     * <p>free + cache/buffers
     * @return Actual Free memory
     */
    public long getActualFree() {
        return actualFree;
    }

    /**
     * Get the Percent total used memory
     * (total - actualFree) / total
     * @return Percent total used memory
     */
    public double getUsedPercent() {
        return usedPercent;
    }

    /**
     * Get the Percent total free memory
     * (total - actualUsed) / total
     * @return Percent total free memory
     */
    public double getFreePercent() {
        return freePercent;
    }

    @Override
    public String toString() {
        return
                "Mem: " +
                (this.total / 1024) + "K total, " +
                (this.mtrr) + "M mtrr, " +
                (this.used / 1024) + "K used, " +
                (this.free / 1024) + "K free, " +
                (this.actualUsed / 1024) + "K actUsed, " +
                (this.actualFree / 1024) + "K actFree, " +
                (String.format("%.3f", this.usedPercent)) + "% used, " +
                (String.format("%.3f", this.freePercent)) + "% free";
    }
}
