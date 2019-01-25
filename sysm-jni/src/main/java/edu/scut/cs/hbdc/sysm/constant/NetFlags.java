package edu.scut.cs.hbdc.sysm.constant;

public class NetFlags {

    private NetFlags() {}

    /**
     * interface is up
     */
    public final static int IFF_UP = 0x1;

    /**
     * broadcast address valid
     */
    public final static int IFF_BROADCAST = 0x2;

    /**
     * debugging is on
     */
    public final static int IFF_DEBUG = 0x4;

    /**
     * is a loopback net
     */
    public final static int IFF_LOOPBACK = 0x8;

    /**
     * interface has a point-to-point link
     */
    public final static int IFF_POINTOPOINT = 0x10;

    /**
     * flags as client
     */
    public final static int CONN_CLIENT = 0x01;
    /**
     * flags as server
     */
    public final static int CONN_SERVER = 0x02;
    /**
     * flags TCP and TCP6
     */
    public final static int CONN_TCP  = 0x10;
    /**
     * flags UDP and UDP6
     */
    public final static int CONN_UDP  = 0x20;
    /**
     * flags RAW and RAW
     */
    public final static int CONN_RAW  = 0x40;
}
