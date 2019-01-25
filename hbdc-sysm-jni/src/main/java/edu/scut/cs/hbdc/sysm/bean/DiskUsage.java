package edu.scut.cs.hbdc.sysm.bean;

import edu.scut.cs.hbdc.sysm.exception.SysmException;
import edu.scut.cs.hbdc.sysm.Sysm;

import java.io.Serializable;

public class DiskUsage extends AbstractBean implements Serializable {

    private static final long serialVersionUID = 5588698372036871730L;
    private long reads;
    private long writes;
    private long readBytes;
    private long writeBytes;
    private double util;
    private double serviceTime;

    public DiskUsage() {}

    /**
     * use the dirName to find the disk e.g. /dev/sda1 mounted on the /home
     * and we can use /home to get the DiskUsage of /dev/sda1
     * @param sysm sysm
     * @param name dirName
     * @throws SysmException SysmException
     */
    public native void gather(Sysm sysm, String name) throws SysmException;

    public static DiskUsage fetch(Sysm sysm, String name) throws SysmException {
        DiskUsage diskUsage = new DiskUsage();
        diskUsage.gather(sysm, name);
        return diskUsage;
    }

    /**
     * Get the Number of physical disk reads
     * @return Number of physical disk reads
     */
    public long getReads() {
        return reads;
    }

    /**
     * Get the Number of physical disk writes
     * @return Number of physical disk writes
     */
    public long getWrites() {
        return writes;
    }

    /**
     * Get the Number of physical disk bytes read
     * @return Number of physical disk bytes read
     */
    public long getReadBytes() {
        return readBytes;
    }

    /**
     * Get the Number of physical disk bytes written
     * @return Number of physical disk bytes written
     */
    public long getWriteBytes() {
        return writeBytes;
    }

    /**
     * Get the util
     * avg io loader like <code>iostat</code> util% but not * 100
     * @return util
     */
    public double getUtil() {
        return util;
    }

    /**
     * Get the service_time (ms) (time spend in IO) / (number of reads and writes)
     * @return service_time
     */
    public double getServiceTime() {
        return serviceTime;
    }
}
