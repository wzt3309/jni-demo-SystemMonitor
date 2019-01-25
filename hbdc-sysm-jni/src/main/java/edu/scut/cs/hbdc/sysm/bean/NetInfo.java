package edu.scut.cs.hbdc.sysm.bean;

import edu.scut.cs.hbdc.sysm.exception.SysmException;
import edu.scut.cs.hbdc.sysm.Sysm;

import java.io.Serializable;

public class NetInfo extends AbstractBean implements Serializable {

    private static final long serialVersionUID = 607069717891507718L;
    private String defaultGateway;
    private String defaultGatewayInterface;
    private String hostName;
    private String domainName;
    private String primaryDns;
    private String secondaryDns;

    public NetInfo() {
    }

    public native void gather(Sysm sysm) throws SysmException;

    public static NetInfo fetch(Sysm sysm) throws SysmException {
        NetInfo netInfo = new NetInfo();
        netInfo.gather(sysm);
        return netInfo;
    }

    /**
     * Get the default_gateway
     * @return default_gateway
     */
    public String getDefaultGateway() {
        return defaultGateway;
    }

    /**
     * Get the default_gateway_interface
     * @return default_gateway_interface
     */
    public String getDefaultGatewayInterface() {
        return defaultGatewayInterface;
    }

    /**
     * Get the host_name
     * @return host_name
     */
    public String getHostName() {
        return hostName;
    }

    /**
     * Get the domain_name
     * @return domain_name
     */
    public String getDomainName() {
        return domainName;
    }

    /**
     * Get the primary_dns
     * @return primary_dns
     */
    public String getPrimaryDns() {
        return primaryDns;
    }

    /**
     * Get the secondary_dns
     * @return secondary_dns
     */
    public String getSecondaryDns() {
        return secondaryDns;
    }
}
