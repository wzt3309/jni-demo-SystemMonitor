package edu.scut.cs.hbdc.sysm.bean;

import edu.scut.cs.hbdc.sysm.exception.SysmException;
import edu.scut.cs.hbdc.sysm.Sysm;

import java.io.Serializable;

public class Uptime extends AbstractBean implements Serializable{

    private static final long serialVersionUID = 6663306996690896859L;
    private double uptime;

    public Uptime() {
    }

    public native void gather(Sysm sysm) throws SysmException;

    public static Uptime fetch(Sysm sysm) throws SysmException {
        Uptime uptime = new Uptime();
        uptime.gather(sysm);
        return uptime;
    }

    /**
     * Get the Time since machine started in seconds
     * @return Time since machine started in secon
     */
    public double getUptime() {
        return uptime;
    }
}
