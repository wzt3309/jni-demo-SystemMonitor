package edu.scut.cs.hbdc.sysm.bean;

import java.io.Serializable;

public class NetRoute extends AbstractBean implements Serializable {

    private static final long serialVersionUID = 2999564394869905284L;
    private String destination;
    private String gateway;
    private String mask;
    private long flags;
    private long metric;
    private long refcnt;
    private long use;
    private String ifname;
    private long mtu;
    private long window;
    private long irtt;

    public NetRoute() {
    }

    /**
     * Get the destination
     * <code>route</code>
     * The destination network or destination host
     * @return destination
     */
    public String getDestination() {
        return destination;
    }

    /**
     * Get the gateway
     * <code>route</code>
     * The gateway address or '*' if none set
     * @return gateway
     */
    public String getGateway() {
        return gateway;
    }

    /**
     * Get the mask
     * <code>route</code>
     * The netmask for the destination net; '255.255.255.255' for a
     * host destination and '0.0.0.0' for the default route.
     * @return mask
     */
    public String getMask() {
        return mask;
    }

    /**
     * Possible flags include
     * <code>route</code>
     * <ul>
     *     <li>U (route is up)</li>
     *     <li>H (target is a host)</li>
     *     <li>G (use gateway)</li>
     *     <li>R (reinstate route for dynamic routing)</li>
     *     <li>D (dynamically installed by daemon or redirect)</li>
     *     <li>H (target is a host)</li>
     *     <li>M (modified from routing daemon or redirect)</li>
     *     <li>A (installed by addrconf)</li>
     *     <li>C (cache entry)</li>
     *     <li>!  (reject route)</li>
     * </ul>
     * @return flags
     */
    public long getFlags() {
        return flags;
    }

    /**
     * Get the metric
     * <code>route</code>
     * The 'distance' to the target (usually counted in  hops). It is
     * not used by recent kernels, but may be needed by routing daemons
     * @return metric
     */
    public long getMetric() {
        return metric;
    }

    /**
     * Get the refcnt
     * <code>route</code>
     * Number of references to this route (Not used in the Linux  kernel)
     * @return refcnt
     */
    public long getRefcnt() {
        return refcnt;
    }

    /**
     * Get the use
     * <code>route</code>
     * Count of lookups for the route. Depending on the use of -F and
     * -C this will be either route cache misses (-F) or hits (-C).
     * @return use
     */
    public long getUse() {
        return use;
    }

    /**
     * Get the ifname
     * <code>route</code>
     * Interface to which packets for this route will be sent
     * @return ifname
     */
    public String getIfname() {
        return ifname;
    }

    /**
     * Get the mtu
     * Default maximum segment size for TCP connections over this
     * route.
     * @return mtu
     */
    public long getMtu() {
        return mtu;
    }

    /**
     * Get the window
     * <code>route</code>
     * Default window size for TCP connections over this route
     * @return window
     */
    public long getWindow() {
        return window;
    }

    /**
     * Get the irtt
     * Initial  RTT  (Round  Trip  Time). The kernel uses this to guess
     * about the best TCP protocol parameters without waiting on  (pos‐
     * sibly slow) answers.
     * @return irtt
     */
    public long getIrtt() {
        return irtt;
    }
}
