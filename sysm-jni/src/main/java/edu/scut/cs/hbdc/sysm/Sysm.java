package edu.scut.cs.hbdc.sysm;

import edu.scut.cs.hbdc.sysm.bean.*;
import edu.scut.cs.hbdc.sysm.constant.NetFlags;
import edu.scut.cs.hbdc.sysm.exception.SysmException;
import edu.scut.cs.hbdc.sysm.jni.ArchLoader;
import edu.scut.cs.hbdc.sysm.jni.ArchLoaderException;
import edu.scut.cs.hbdc.sysm.jni.ArchNotSupportedException;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import java.util.Map;
import java.util.StringTokenizer;

/**
 * Sysm object keep the pointer(32/64bit address) to <code>sysm_t *sysm</code>
 * containing sysmtem information in native calls. And we also use Java's dynamic
 * proxy mechanism to use a cache object in order to reducing native calls
 */
public class Sysm implements SysmProxy{
    public static final String NATIVE_VERSION_STRING = "1.0.0";

    private static final Logger log = LoggerFactory.getLogger(Sysm.class);

    private static String loadError = null;
    private static ArchLoader loader;
    private static Sysm singleton;

    private boolean open = false;
    //store jni_sysm_t* sysm in order to get a sysm_t instance which has all system information
    private int sysmWrapper = 0;
    //same, but if system is 64bit the sizeof(void *) is long
    private long longSysmWrapper = 0;
    // lastCpu is used to calculate the cpu percentage;
    private Cpu lastCpu;
    private Cpu[] lastCpuList;

    static void init(ArchLoader archLoader, String path) {
        loader = archLoader;
        String nativeLibVersion = "unknown";

        try {
            loadLibrary(path);
            nativeLibVersion = getNativeLibVersion();
            checkVersion(nativeLibVersion);
        } catch (SysmException e) {
            loadError = e.getMessage();
            log.error(loadError, e);
        }
    }

    private static void loadLibrary(String path) throws SysmException {
        try {
            loader.load(path);
        } catch (ArchNotSupportedException
                | ArchLoaderException
                | UnsatisfiedLinkError e) {
            throw new SysmException(e.getMessage());
        }
    }

    private static native String getNativeLibVersion();

    private static void checkVersion(String nativeVersionString)
            throws SysmException {
        StringTokenizer version =
                new StringTokenizer(NATIVE_VERSION_STRING, ".");
        StringTokenizer realVersion =
                new StringTokenizer(nativeVersionString, ".");
        String[] desc = {"major", "minor"};

        for (String var: desc) {
            String v =
                    version.hasMoreTokens() ?
                            version.nextToken() : "0";
            String rv =
                    realVersion.hasMoreTokens() ?
                            realVersion.nextToken() : "0";
            if (!v.equals(rv)) {
                String msg = var + " version mismatch: (" +
                        v + "!=" + rv + ")" +
                        "need version=" + NATIVE_VERSION_STRING +
                        ", real version=" + nativeVersionString;
                throw new SysmException();
            }
        }
    }

    public static Sysm getInstance() {
        return getInstance(new SysmLoader(), null);
    }

    public static Sysm getInstance(String path) {
        return getInstance(new SysmLoader(), path);
    }

    public static Sysm getInstance(ArchLoader loader, String path) {
        if (singleton == null) {
            init(loader, path);
            singleton = new Sysm();
        }
        return singleton;
    }

    private Sysm() {
        try {
            open();
        } catch (SysmException | UnsatisfiedLinkError e) {
            log.error(e.getMessage(), e);
        }
    }

    private synchronized void open() throws SysmException {
        if (!this.open) {
            nativeOpen();
            this.open = true;
        }
    }

    private native void nativeOpen() throws SysmException;

    public synchronized void close() {
        if (this.open) {
            nativeClose();
            this.open = false;
        }
    }

    private native int nativeClose();

    /**
     * Send a signal to a process.
     * @param pid process id
     * @param signal signal
     * @throws SysmException SysmException
     */
    public native void signal(long pid, int signal) throws SysmException;

    /**
     * Get pid of the current process.
     * @return Pid of the current process
     */
    @Override
    public native long getPid() throws SysmException;

    /**
     * Memory state
     * @return Memory state
     * @throws SysmException SysmException
     */
    @Override
    public Memory getMem() throws SysmException {
        return Memory.fetch(this);
    }

    /**
     * Swap state
     * @return Swap state
     * @throws SysmException SysmException
     */
    @Override
    public Swap getSwap() throws SysmException {
        return Swap.fetch(this);
    }

    /**
     * Cpu state
     * @return Cpu state
     * @throws SysmException SysmException
     */
    @Override
    public Cpu getCpu() throws SysmException {
        return Cpu.fetch(this);
    }

    /**
     * Cpu Percentage
     * e.g. diffUser = curCpu.user - oldCpu.user;
     *      differTotal = curCpu.total - oldCpu.total;
     *      cupPerc.user = diffUser / diffTotal
     * @return Cpu Percentage
     * @throws SysmException SysmException
     */
    @Override
    public CpuPercentage getCpuPercentage() throws SysmException {
        Cpu oldCpu;

        if (this.lastCpu == null) {
            oldCpu = getCpu();
            try {
                Thread.sleep(500);
            } catch (InterruptedException e) {
                log.warn(e.getMessage());
            }
        } else {
            oldCpu = this.lastCpu;
        }
        this.lastCpu = getCpu();
        return CpuPercentage.fetch(this, oldCpu, this.lastCpu);
    }

    /**
     * All Cpus Infomation
     * @return All Cpus Infomation
     * @throws SysmException SysmException
     */
    @Override
    public native CpuInfo[] getCpuInfos() throws SysmException;

    /**
     * All Cpus state
     * @return All Cpus state
     * @throws SysmException SysmException
     */
    @Override
    public Cpu[] getCpus() throws SysmException {
        return getCpusNative();
    }

    private native Cpu[] getCpusNative() throws SysmException;

    /**
     * All Cpus' Percentage
     * @return All Cpus' Percentage
     * @throws SysmException SysmException
     */
    @Override
    public CpuPercentage[] getCpuPercentages() throws SysmException {
        Cpu[] oldCpuList;

        if (this.lastCpuList == null) {
            oldCpuList = getCpus();
            try {
                Thread.sleep(500);
            } catch (InterruptedException e) {
                log.warn(e.getMessage());
            }
        } else {
            oldCpuList = this.lastCpuList;
        }

        this.lastCpuList = getCpus();

        int curLen = this.lastCpuList.length;
        int oldLen = oldCpuList.length;

        CpuPercentage[] perc = new CpuPercentage[curLen < oldLen ? curLen : oldLen];

        for (int i = 0;i < perc.length;i++) {
            perc[i] = CpuPercentage.fetch(this, oldCpuList[i], this.lastCpuList[i]);
        }

        return perc;
    }

    @Override
    public Uptime getUptime() throws SysmException {
        return Uptime.fetch(this);
    }

    @Override
    public ResourceLimit getResourceLimit() throws SysmException {
        return ResourceLimit.fetch(this);
    }

    /**
     * Get system load average.
     * @return The system load averages for the past 1, 5, and 15 minutes.
     * @throws SysmException SysmException
     */
    @Override
    public native double[] getLoadAverage() throws SysmException;

    /**
     * Process list
     * @return process list
     * @throws SysmException SysmException
     */
    @Override
    public native long[] getProcesses() throws SysmException;

    @Override
    public ProcessStat getProcessStat() throws SysmException {
        return ProcessStat.fetch(this);
    }

    @Override
    public ProcessMem getProcessMem(long pid) throws SysmException {
        return ProcessMem.fetch(this, pid);
    }

    @Override
    public ProcessMem getMultiProcessMem(String query) {
        return null;
    }

    @Override
    public ProcessState getProcessState(long pid) throws SysmException {
        return ProcessState.fetch(this, pid);
    }

    @Override
    public ProcessState getMultiProcessState(String query) {
        return null;
    }

    @Override
    public ProcessTime getProcessTime(long pid) throws SysmException {
        return ProcessTime.fetch(this, pid);
    }

    @Override
    public ProcessCpu getProcessCpu(long pid) throws SysmException {
        return ProcessCpu.fetch(this, pid);
    }

    @Override
    public MultiProcessCpu getMultiProcessCpu(String query) {
        return null;
    }

    @Override
    public ProcessOwn getProcessOwn(long pid) throws SysmException {
        return ProcessOwn.fetch(this, pid);
    }

    @Override
    public ProcessOwner getProcessOwner(long pid) throws SysmException {
        return ProcessOwner.fetch(this, pid);
    }

    @Override
    public ProcessExe getProcessExe(long pid) throws SysmException {
        return ProcessExe.fetch(this, pid);
    }

    /**
     * Process's args e.g. a process(ls -l) return "ls -l"
     * @param pid pid
     * @return Process args
     */
    @Override
    public native String[] getProcessArgs(long pid);

    @Override
    public Map<String, String> getProcessEnv(long pid) throws SysmException {
        return ProcessEnv.getAll(this, pid);
    }

    @Override
    public String getProcessEnv(long pid, String key) throws SysmException {
        return ProcessEnv.getValue(this, pid, key);
    }

    /**
     * Find pid by protocol and port(just like <code>netstat</code> in linux)
     * @param protocol tcp/udp/raw
     * @param port local port
     * @return Pid if doesn't exist return 0
     * @throws SysmException SysmException
     */
    @Override
    public native long getProcessPort(int protocol, long port) throws SysmException;

    @Override
    public DiskUsage getDiskUsage(String name) throws SysmException {
        if (name == null) {
            throw new SysmException("name can't be null");
        }
        return DiskUsage.fetch(this, name);
    }

    @Override
    public NetInfo getNetInfo() throws SysmException {
        return NetInfo.fetch(this);
    }

    @Override
    public NetStat getNetStat() throws SysmException {
        NetStat netStat = new NetStat();
        netStat.stat(this);
        return netStat;
    }

    @Override
    public NetStat getNetStat(byte[] address, long port) throws SysmException {
        NetStat netStat = new NetStat();
        netStat.stat(this, address, port);
        return netStat;
    }

    @Override
    public native NetRoute[] getNetRoutes() throws SysmException;

    @Override
    public NetInterfaceConfig getNetInterfaceConfig() throws SysmException {
        String[] interfaces = getNetInterfaces();

        for (String ifname : interfaces) {
            NetInterfaceConfig ifconfig;

            try {
                ifconfig = getNetInterfaceConfig(ifname);
            } catch (SysmException e) {
                continue;
            }

            long flags = ifconfig.getFlags();
            if ((flags & NetFlags.IFF_UP) <= 0) continue;
            if ((flags & NetFlags.IFF_POINTOPOINT) > 0) continue;
            if ((flags & NetFlags.IFF_LOOPBACK) > 0) continue;

            return ifconfig;
        }

        String msg = "No ethernet interface available";
        throw new SysmException(msg);
    }

    @Override
    public NetInterfaceConfig getNetInterfaceConfig(String ifname) throws SysmException {
        return NetInterfaceConfig.fetch(this, ifname);
    }

    @Override
    public NetInterfaceStat getNetInterfaceStat(String ifname) throws SysmException {
        return NetInterfaceStat.fetch(this, ifname);
    }

    @Override
    public native String[] getNetInterfaces() throws SysmException;

    @Override
    public native NetConnection[] getNetConnections(int flags) throws SysmException;

    @Override
    public native String getNetListenAddress(long port) throws SysmException;

    @Override
    public native Who[] getWhos() throws SysmException;

    @Override
    protected void finalize() throws Throwable {
        close();
    }
}
