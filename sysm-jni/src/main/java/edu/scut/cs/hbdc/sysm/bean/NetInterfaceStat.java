package edu.scut.cs.hbdc.sysm.bean;

import edu.scut.cs.hbdc.sysm.exception.SysmException;
import edu.scut.cs.hbdc.sysm.Sysm;

import java.io.Serializable;

public class NetInterfaceStat extends AbstractBean implements Serializable {

    private static final long serialVersionUID = -6283383811559689540L;
    private long rxBytes;
    private long rxPackets;
    private long rxErrors;
    private long rxDropped;
    private long rxOverruns;
    private long rxFrame;
    private long txBytes;
    private long txPackets;
    private long txErrors;
    private long txDropped;
    private long txOverruns;
    private long txCollisions;
    private long txCarrier;
    private long speed;

    public NetInterfaceStat() {
    }

    public native void gather(Sysm sysm, String ifname) throws SysmException;

    public static NetInterfaceStat fetch(Sysm sysm, String ifname) throws SysmException {
        NetInterfaceStat netInterfaceStat = new NetInterfaceStat();
        netInterfaceStat.gather(sysm, ifname);
        return netInterfaceStat;
    }
    /**
     * Get the rx_bytes (receive)
     * @return rx_bytes
     */
    public long getRxBytes() {
        return rxBytes;
    }

    /**
     * Get the rx_packets (receive)
     * @return rx_packets
     */
    public long getRxPackets() {
        return rxPackets;
    }

    /**
     * Get the rx_errors (receive)
     * @return rx_errors
     */
    public long getRxErrors() {
        return rxErrors;
    }

    /**
     * Get the rx_dropped (receive)
     * @return rx_dropped
     */
    public long getRxDropped() {
        return rxDropped;
    }

    /**
     * Get the rx_overruns (receive)
     * @return rx_overruns
     */
    public long getRxOverruns() {
        return rxOverruns;
    }

    /**
     * Get the rx_frame (receive)
     * @return rx_frame
     */
    public long getRxFrame() {
        return rxFrame;
    }

    /**
     * Get the tx_bytes (transmission)
     * @return tx_bytes
     */
    public long getTxBytes() {
        return txBytes;
    }

    /**
     * Get the tx_packets (transmission)
     * @return tx_packets
     */
    public long getTxPackets() {
        return txPackets;
    }

    /**
     * Get the tx_errors (transmission)
     * @return tx_errors
     */
    public long getTxErrors() {
        return txErrors;
    }

    /**
     * Get the tx_dropped (transmission)
     * @return tx_dropped
     */
    public long getTxDropped() {
        return txDropped;
    }

    /**
     * Get the tx_overruns (transmission)
     * @return tx_overruns
     */
    public long getTxOverruns() {
        return txOverruns;
    }

    /**
     * Get the tx_collisions (transmission)
     * @return tx_collisions
     */
    public long getTxCollisions() {
        return txCollisions;
    }

    /**
     * Get the tx_carrier (transmission)
     * @return tx_carrier
     */
    public long getTxCarrier() {
        return txCarrier;
    }

    /**
     * Get the speed
     * @return speed
     */
    public long getSpeed() {
        return speed;
    }
}
