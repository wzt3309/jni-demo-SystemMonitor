package edu.scut.cs.hbdc.sysm.bean;

import java.io.Serializable;

public class Who extends AbstractBean implements Serializable {

    private static final long serialVersionUID = 4180371418674340055L;
    private String user;
    private String device;
    private String host;
    private long time;

    public Who() {
    }

    /**
     * Get the user
     * @return user
     */
    public String getUser() {
        return user;
    }

    /**
     * Get the login terminal
     * @return login terminal
     */
    public String getDevice() {
        return device;
    }

    /**
     * Get the login host address
     * @return the login host address
     */
    public String getHost() {
        return host;
    }

    /**
     * Get the time
     * @return time
     */
    public long getTime() {
        return time;
    }
}
