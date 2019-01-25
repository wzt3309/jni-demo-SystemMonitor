package edu.scut.cs.hbdc.sysm.bean;

import edu.scut.cs.hbdc.sysm.exception.SysmException;
import edu.scut.cs.hbdc.sysm.Sysm;

import java.io.Serializable;

public class ProcessState extends AbstractBean implements Serializable {

    private static final long serialVersionUID = 6682856328296314457L;
    private char state;
    private String name;
    private long ppid;
    private int tty;
    private int nice;
    private int priority;
    private long threads;
    private int processor;

    public static final char SLEEP  = 'S';
    public static final char RUN    = 'R';
    public static final char STOP   = 'T';
    public static final char ZOMBIE = 'Z';
    public static final char IDLE   = 'D';

    public ProcessState() {
    }

    public native void gather(Sysm sysm, long pid) throws SysmException;

    public static ProcessState fetch(Sysm sysm, long pid) throws SysmException {
        ProcessState processState = new ProcessState();
        processState.gather(sysm, pid);
        return processState;
    }

    /**
     * Get the Process state (Running, Zombie, etc.)
     * @return Process state (Running, Zombie, etc.)
     */
    public char getState() {
        return state;
    }

    /**
     * Get the Name of the process program
     * @return Name of the process program
     */
    public String getName() {
        return name;
    }

    /**
     * Get the Process parent process id
     * @return Process parent process id
     */
    public long getPpid() {
        return ppid;
    }

    /**
     * Get the Device number of rocess controling terminal
     * @return Device number of rocess controling terminal
     */
    public int getTty() {
        return tty;
    }

    /**
     * Get the Nice value of process
     * ps -l -> NI
     * @return Nice value of process
     */
    public int getNice() {
        return nice;
    }

    /**
     * Get the Kernel scheduling priority of process
     * ps -l -> PRI
     * @return Kernel scheduling priority of process
     */
    public int getPriority() {
        return priority;
    }

    /**
     * Get the Number of active threads
     * @return Number of active threads
     */
    public long getThreads() {
        return threads;
    }

    /**
     * Get the Processor number last run on
     * @return Processor number last run on
     */
    public int getProcessor() {
        return processor;
    }
}
