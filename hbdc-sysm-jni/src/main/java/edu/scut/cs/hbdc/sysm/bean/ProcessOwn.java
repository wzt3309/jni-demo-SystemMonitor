package edu.scut.cs.hbdc.sysm.bean;

import edu.scut.cs.hbdc.sysm.exception.SysmException;
import edu.scut.cs.hbdc.sysm.Sysm;

import java.io.Serializable;

public class ProcessOwn extends AbstractBean implements Serializable {

    private static final long serialVersionUID = 2125414655040821585L;
    private long uid;
    private long gid;
    private long euid;
    private long egid;

    public ProcessOwn() {
    }

    public native void gather(Sysm sysm, long pid) throws SysmException;

    public static ProcessOwn fetch(Sysm sysm, long pid) throws SysmException {
        ProcessOwn processOwn = new ProcessOwn();
        processOwn.gather(sysm, pid);
        return processOwn;
    }
    /**
     * Get the Process user id
     * @return Process user id
     */
    public long getUid() {
        return uid;
    }

    /**
     * Get the Process group id
     * @return Process group id
     */
    public long getGid() {
        return gid;
    }

    /**
     * Get the Process effective user id
     * @return Process effective user id
     */
    public long getEuid() {
        return euid;
    }

    /**
     * Get the Process effective group id
     * @return Process effective group id
     */
    public long getEgid() {
        return egid;
    }
}
