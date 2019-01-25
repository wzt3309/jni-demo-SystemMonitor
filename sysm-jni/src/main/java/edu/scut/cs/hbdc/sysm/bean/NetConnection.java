package edu.scut.cs.hbdc.sysm.bean;

import java.io.Serializable;

public class NetConnection extends AbstractBean implements Serializable {

    private static final long serialVersionUID = 8431951154960807027L;
    private long localPort;
    private String localAddress;
    private long remotePort;
    private String remoteAddress;
    private int type;
    private int state;
    private long sendQueue;
    private long receiveQueue;

    public NetConnection() {
    }

    /**
     * Get the local_port
     * <code>netstat</code>
     * @return local_port
     */
    public long getLocalPort() {
        return localPort;
    }

    /**
     * Get the local_address
     * <code>netstat</code>
     * @return local_address
     */
    public String getLocalAddress() {
        return localAddress;
    }

    /**
     * Get the remote_port
     * <code>netstat</code>
     * @return remote_port
     */
    public long getRemotePort() {
        return remotePort;
    }

    /**
     * Get the remote_address
     * <code>netstat</code>
     * @return remote_address
     */
    public String getRemoteAddress() {
        return remoteAddress;
    }

    /**
     * Get the type
     * <code>netstat</code>
     * @return type
     */
    public int getType() {
        return type;
    }

    /**
     * Get the state
     * @return state
     */
    public int getState() {
        return state;
    }

    /**
     * Get the send_queue
     * @return send_queue
     */
    public long getSendQueue() {
        return sendQueue;
    }

    /**
     * Get the receive_queue
     * @return receive_queue
     */
    public long getReceiveQueue() {
        return receiveQueue;
    }
}
