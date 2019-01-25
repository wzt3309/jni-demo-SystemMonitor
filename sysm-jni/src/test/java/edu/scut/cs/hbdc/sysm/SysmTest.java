package edu.scut.cs.hbdc.sysm;

import edu.scut.cs.hbdc.sysm.bean.*;
import edu.scut.cs.hbdc.sysm.constant.NetFlags;
import edu.scut.cs.hbdc.sysm.exception.SysmException;
import org.junit.After;
import org.junit.Before;
import org.junit.Test;

import java.util.Arrays;

public class SysmTest {
    private Sysm sysm = Sysm.getInstance();
    private SysmProxy sysmProxy;
    private long pid;

    @Before
    public void setSysmProxy() throws SysmException {
        sysmProxy = SysmProxyCache.newInstance(sysm);
        pid = sysmProxy.getPid();
    }

    @After
    public void resetSysmProxy() {
        sysmProxy = SysmProxyCache.newInstance(sysm);
    }

    @Test
    public void getPid() throws Exception {
        pid = sysmProxy.getPid();
        System.out.println(pid);
    }

    @Test
    public void getCpuInfos() throws Exception {
        CpuInfo[] cpuInfos = sysmProxy.getCpuInfos();
        Arrays.stream(cpuInfos).forEach(System.out::println);
    }

    @Test
    public void getCpus() throws Exception {
        Cpu[] cpus = sysmProxy.getCpus();
        Arrays.stream(cpus).forEach(System.out::println);
    }

    @Test
    public void getMemory() throws Exception {
        Memory mem = sysmProxy.getMem();
        System.out.println(mem);
    }

    @Test
    public void getSwap() throws Exception {
        Swap swap = sysmProxy.getSwap();
        System.out.println(swap);
    }

    @Test
    public void getCpu() throws Exception {
        Cpu cpu = sysmProxy.getCpu();
        System.out.println(cpu);
    }

    @Test
    public void getCpuPercentage() throws Exception {
        int count = 0;
        sysmProxy = SysmProxyCache.newInstance(sysm, 100);
        while (true) {
            CpuPercentage cpuPercentage = sysmProxy.getCpuPercentage();
            System.out.println(cpuPercentage);
            Thread.sleep(2000);
            if (++count > 9)
                break;
        }
    }

    @Test
    public void getLoadavg() throws Exception {
        int count = 0;
        sysmProxy = SysmProxyCache.newInstance(sysm, 100);
        while (true) {
            System.out.println(Arrays.toString(sysmProxy.getLoadAverage()));
            Thread.sleep(2000);
            if (++count > 9)
                break;
        }
    }

    @Test
    public void getProcesses() throws Exception {
        System.out.println(Arrays.toString(sysmProxy.getProcesses()));
    }

    @Test
    public void getProcessArgs() throws Exception {
        Arrays.stream(sysmProxy.getProcessArgs(pid)).forEach(System.out::println);
    }

    @Test
    public void getProcessPort() throws Exception {
        System.out.println(sysmProxy.getProcessPort(NetFlags.CONN_TCP, 34411));
    }

    @Test
    public void getProcessCpu() throws Exception {
        System.out.println(pid);
        sysmProxy = SysmProxyCache.newInstance(sysm, 100);
        int count = 0;
        while (true) {
            System.out.println(sysmProxy.getProcessCpu(pid));
            Thread.sleep(2000);
            if (++count > 9)
                break;
        }
    }

    @Test
    public void getProcessEnv() throws Exception {
        System.out.println(sysmProxy.getProcessEnv(pid));
    }

    @Test
    public void getProcessEnvVal() throws Exception {

        System.out.println(sysmProxy.getProcessEnv(pid, "HOME"));
    }

    @Test
    public void getProcessExe() throws Exception {
        System.out.println(sysmProxy.getProcessExe(pid));
    }

    @Test
    public void getProcessMem() throws Exception {
        System.out.println(sysmProxy.getProcessMem(pid));
    }

    @Test
    public void getDiskUsage() throws Exception {
        int count = 0;
        sysmProxy = SysmProxyCache.newInstance(sysm, 100);
        while (true) {
            System.out.println(sysmProxy.getDiskUsage("/dev/sda"));
            Thread.sleep(1000);
            if (++count > 99)
                break;
        }
    }

    @Test
    public void getNetInfo() throws Exception {
        System.out.println(sysmProxy.getNetInfo());
    }

    @Test
    public void getNetStat() throws Exception {
        System.out.println(sysmProxy.getNetStat());
        System.out.println(sysmProxy.getNetStat(new byte[]{127, 0, 0, 1}, 34411));
    }

    @Test
    public void getNetInterfaceConfig() throws Exception {
        System.out.println(sysmProxy.getNetInterfaceConfig("ens33"));
    }

    @Test
    public void getNetInterfaceStat() throws Exception {
        System.out.println(sysmProxy.getNetInterfaceStat("ens33"));
    }

    @Test
    public void getNetConnections() throws Exception {
        for (NetConnection conn:
                sysmProxy.getNetConnections( NetFlags.CONN_SERVER | NetFlags.CONN_CLIENT |
                        NetFlags.CONN_TCP)) {
            System.out.println(conn);
        }
    }

    @Test
    public void getNetListenAddress() throws Exception {
        System.out.println(sysmProxy.getNetListenAddress(39597));
    }

    @Test
    public void getWhos() throws Exception {
        for (Who who: sysmProxy.getWhos()) {
            System.out.println(who);
        }
    }
}