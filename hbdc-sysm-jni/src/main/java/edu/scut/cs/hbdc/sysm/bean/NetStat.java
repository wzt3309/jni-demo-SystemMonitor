package edu.scut.cs.hbdc.sysm.bean;

import edu.scut.cs.hbdc.sysm.constant.NetFlags;
import edu.scut.cs.hbdc.sysm.exception.SysmException;
import edu.scut.cs.hbdc.sysm.Sysm;

import java.io.Serializable;

public class NetStat extends AbstractBean implements Serializable {

    private static final long serialVersionUID = 8431951154960807027L;

    private int[] tcpStates;
    private int tcpInboundTotal;
    private int tcpOutboundTotal;
    private int allInboundTotal;
    private int allOutboundTotal;

    public NetStat() {
    }

    public native void stat(Sysm sysm, int flags, byte[] address, long port) throws SysmException;

    public void stat(Sysm sysm) throws SysmException {
        int flags =
                NetFlags.CONN_SERVER | NetFlags.CONN_CLIENT |
                NetFlags.CONN_TCP;
        stat(sysm, flags, null, -1);
    }

    public void stat(Sysm sysm, byte[] address, long port) throws SysmException {
        int flags = NetFlags.CONN_CLIENT | NetFlags.CONN_TCP;
        stat(sysm, flags, address, port);
    }

    public int[] getTcpStates() {
        return tcpStates;
    }

    public int getTcpInboundTotal() {
        return tcpInboundTotal;
    }

    public int getTcpOutboundTotal() {
        return tcpOutboundTotal;
    }

    public int getAllInboundTotal() {
        return allInboundTotal;
    }

    public int getAllOutboundTotal() {
        return allOutboundTotal;
    }
}
