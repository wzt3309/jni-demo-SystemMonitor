package edu.scut.cs.hbdc.sysm.bean;

import edu.scut.cs.hbdc.sysm.exception.SysmException;
import edu.scut.cs.hbdc.sysm.Sysm;

import java.io.Serializable;

public class ProcessStat extends AbstractBean implements Serializable {

    private static final long serialVersionUID = -1262624303422210931L;
    private long total;
    private long idle;
    private long running;
    private long sleeping;
    private long stopped;
    private long zombie;
    private long threads;

    public ProcessStat() {
    }

    public native void gather(Sysm sysm) throws SysmException;

    public static ProcessStat fetch(Sysm sysm) throws SysmException {
        ProcessStat processStat = new ProcessStat();
        processStat.gather(sysm);
        return processStat;
    }
    /**
     * Get the Total number of processes
     * @return Total number of processes
     */
    public long getTotal() {
        return total;
    }

    /**
     * Get the Total number of processes in idle state
     * @return Total number of processes in idle state
     */
    public long getIdle() {
        return idle;
    }

    /**
     * Get the Total number of processes in run state
     * @return Total number of processes in run state
     */
    public long getRunning() {
        return running;
    }

    /**
     * Get the Total number of processes in sleep state
     * @return Total number of processes in sleep state
     */
    public long getSleeping() {
        return sleeping;
    }

    /**
     * Get the Total number of processes in stop state
     * @return Total number of processes in stop state
     */
    public long getStopped() {
        return stopped;
    }

    /**
     * Get the Total number of processes in zombie state
     * @return Total number of processes in zombie state
     */
    public long getZombie() {
        return zombie;
    }

    /**
     * Get the Total number of threads
     * @return Total number of threads
     */
    public long getThreads() {
        return threads;
    }
}
