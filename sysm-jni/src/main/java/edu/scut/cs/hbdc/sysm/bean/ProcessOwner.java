package edu.scut.cs.hbdc.sysm.bean;

import edu.scut.cs.hbdc.sysm.exception.SysmException;
import edu.scut.cs.hbdc.sysm.Sysm;

import java.io.Serializable;

public class ProcessOwner extends AbstractBean implements Serializable {

    private static final long serialVersionUID = 467731875357745897L;
    private String user;
    private String group;

    public ProcessOwner() {
    }

    public native void gather(Sysm sysm, long pid) throws SysmException;

    public static ProcessOwner fetch(Sysm sysm, long pid) throws SysmException {
        ProcessOwner processOwner = new ProcessOwner();
        processOwner.gather(sysm, pid);
        return processOwner;
    }

    /**
     * Get the Process owner user name
     * @return Process owner user name
     */
    public String getUser() {
        return user;
    }

    /**
     * Get the Process owner group name
     * @return Process owner group name
     */
    public String getGroup() {
        return group;
    }
}
