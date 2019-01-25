package edu.scut.cs.hbdc.sysm.bean;

import edu.scut.cs.hbdc.sysm.exception.SysmException;
import edu.scut.cs.hbdc.sysm.Sysm;

import java.io.Serializable;

public class NetInterfaceConfig extends AbstractBean implements Serializable {

    private static final long serialVersionUID = 7301426444308634132L;
    private String name;
    private String hwaddr;
    private String type;
    private String description;
    private String address;
    private String address6;
    private int prefix6Length;
    private int scope6;
    private String destination;
    private String broadcast;
    private String netmask;
    private long flags;
    private long mtu;
    private long metric;
    private int txQueueLen;

    public NetInterfaceConfig() {
    }

    public native void gather(Sysm sysm, String ifname) throws SysmException;

    public static NetInterfaceConfig fetch(Sysm sysm, String ifname) throws SysmException {
        NetInterfaceConfig netInterfaceConfig = new NetInterfaceConfig();
        netInterfaceConfig.gather(sysm, ifname);
        return netInterfaceConfig;
    }

    /**
     * Get the network interface name
     * @return name
     */
    public String getName() {
        return name;
    }

    /**
     * Get the hardware address
     * @return hwaddr
     */
    public String getHwaddr() {
        return hwaddr;
    }

    /**
     * Get the type
     * @return type
     */
    public String getType() {
        return type;
    }

    /**
     * Get the description
     * @return description
     */
    public String getDescription() {
        return description;
    }

    /**
     * Get the address
     * @return address
     */
    public String getAddress() {
        return address;
    }

    /**
     * Get ipv6 address
     * @return ipv6 address
     */
    public String getAddress6() {
        return address6;
    }

    /**
     * Get ipv6 prefix length
     * @return ipv6 prefix length
     */
    public int getPrefix6Length() {
        return prefix6Length;
    }

    /**
     * Get scope (ifconfig -a)
     * @return scope
     */
    public int getScope6() {
        return scope6;
    }

    /**
     * Get the destination
     * <if.h> struct ifreq->ifru_dstaddr
     * @return destination
     */
    public String getDestination() {
        return destination;
    }

    /**
     * Get the broadcast
     * @return broadcast
     */
    public String getBroadcast() {
        return broadcast;
    }

    /**
     * Get the netmask
     * @return netmask
     */
    public String getNetmask() {
        return netmask;
    }

    /**
     * Get the flags
     * <if.h> struct ifreq->ifru_flags
     * @return flags
     */
    public long getFlags() {
        return flags;
    }

    /**
     * Get the mtu
     * @return mtu
     */
    public long getMtu() {
        return mtu;
    }

    /**
     * Get the metric
     * @return metric
     */
    public long getMetric() {
        return metric;
    }

    /**
     * Get the Tx queue length
     * @return the Tx queue length
     */
    public int getTxQueueLen() {
        return txQueueLen;
    }
}
