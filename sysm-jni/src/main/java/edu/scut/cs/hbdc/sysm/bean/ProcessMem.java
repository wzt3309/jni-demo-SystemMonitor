package edu.scut.cs.hbdc.sysm.bean;

import edu.scut.cs.hbdc.sysm.exception.SysmException;
import edu.scut.cs.hbdc.sysm.Sysm;

import java.io.Serializable;

public class ProcessMem extends AbstractBean implements Serializable {

    private static final long serialVersionUID = 4295190486462289477L;
    private long size;
    private long resident;
    private long share;
    private long minorFaults;
    private long majorFaults;
    private long pageFaults;

    public ProcessMem() {
    }

    public native void gather(Sysm sysm, long pid) throws SysmException;

    public static ProcessMem fetch(Sysm sysm, long pid) throws SysmException {
        ProcessMem processMem = new ProcessMem();
        processMem.gather(sysm, pid);
        return processMem;
    }

    /**
     * Get the Total process virtual memory
     * Total program size(same as VmSize in /proc/[pid]/status)
     * @return Total process virtual memory
     */
    public long getSize() {
        return size;
    }

    /**
     * Get the Total process resident memory
     * In computing, resident set size (RSS) is the
     * portion of memory occupied by a process that
     * is held in main memory (RAM).
     * @return Total process resident memory
     */
    public long getResident() {
        return resident;
    }

    /**
     * Get the Total process shared memory
     * @return Total process shared memory
     */
    public long getShare() {
        return share;
    }

    /**
     * Get the non i/o page faults
     * @return non i/o page faults
     */
    public long getMinorFaults() {
        return minorFaults;
    }

    /**
     * Get the i/o page faults
     * @return i/o page faults
     */
    public long getMajorFaults() {
        return majorFaults;
    }

    /**
     * Get the Total number of page faults
     * @return Total number of page faults
     */
    public long getPageFaults() {
        return pageFaults;
    }
}
