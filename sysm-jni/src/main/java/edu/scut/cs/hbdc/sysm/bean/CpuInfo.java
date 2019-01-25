package edu.scut.cs.hbdc.sysm.bean;

import java.io.Serializable;

public class CpuInfo extends AbstractBean implements Serializable {

    private static final long serialVersionUID = -4993776368233208583L;
    private String vendor;
    private String model;
    private int mhz;
    private int mhzMax;
    private int mhzMin;
    private long cacheSize;
    private int totalCores;
    private int totalSockets;
    private int coresPerSocket;

    public CpuInfo() {
    }

    /**
     * Get the CPU vendor id
     * @return CPU vendor id
     */
    public String getVendor() {
        return vendor;
    }

    /**
     * Get the CPU model
     * @return CPU model
     */
    public String getModel() {
        return model;
    }

    /**
     * Get the CPU speed
     * @return CPU speed
     */
    public int getMhz() {
        return mhz;
    }

    /**
     * Get the Max CPU speed
     * @return Max CPU speed
     */
    public int getMhzMax() {
        return mhzMax;
    }

    /**
     * Get the Min CPU speed
     * @return Min CPU speed
     */
    public int getMhzMin() {
        return mhzMin;
    }

    /**
     * Get the CPU cache size
     * @return CPU cache size
     */
    public long getCacheSize() {
        return cacheSize;
    }

    /**
     * Get the Total CPU cores (logical)
     * @return Total CPU cores (logical)
     */
    public int getTotalCores() {
        return totalCores;
    }

    /**
     * Get the Total CPU sockets (physical)
     * @return Total CPU sockets (physical)
     */
    public int getTotalSockets() {
        return totalSockets;
    }

    /**
     * Get the Number of CPU cores per CPU socket
     * @return Number of CPU cores per CPU socket
     */
    public int getCoresPerSocket() {
        return coresPerSocket;
    }
}
