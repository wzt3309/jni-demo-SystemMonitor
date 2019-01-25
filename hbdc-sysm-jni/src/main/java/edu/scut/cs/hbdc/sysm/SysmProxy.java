package edu.scut.cs.hbdc.sysm;

import edu.scut.cs.hbdc.sysm.bean.*;
import edu.scut.cs.hbdc.sysm.exception.SysmException;

import java.util.Map;

public interface SysmProxy {
    long getPid() throws SysmException;

    Memory getMem() throws SysmException;

    Swap getSwap() throws SysmException;

    Cpu getCpu() throws SysmException;

    CpuPercentage getCpuPercentage() throws SysmException;

    CpuInfo[] getCpuInfos() throws SysmException;

    Cpu[] getCpus() throws SysmException;

    CpuPercentage[] getCpuPercentages() throws SysmException;

    Uptime getUptime() throws SysmException;

    ResourceLimit getResourceLimit() throws SysmException;

    double[] getLoadAverage() throws SysmException;

    long[] getProcesses() throws SysmException;

    ProcessStat getProcessStat() throws SysmException;

    ProcessMem getProcessMem(long pid) throws SysmException;

    ProcessMem getMultiProcessMem(String query) throws SysmException;

    ProcessState getProcessState(long pid) throws SysmException;

    ProcessState getMultiProcessState(String query);

    ProcessTime getProcessTime(long pid) throws SysmException;

    ProcessCpu getProcessCpu(long pid) throws SysmException;

    MultiProcessCpu getMultiProcessCpu(String query);

    ProcessOwn getProcessOwn(long pid) throws SysmException;

    ProcessOwner getProcessOwner(long pid) throws SysmException;

    ProcessExe getProcessExe(long pid) throws SysmException;

    String[] getProcessArgs(long pid);

    Map<String, String> getProcessEnv(long pid) throws SysmException;

    String getProcessEnv(long pid, String key) throws SysmException;

    long getProcessPort(int protocol, long port) throws SysmException;

    DiskUsage getDiskUsage(String name) throws SysmException;

    NetInfo getNetInfo() throws SysmException;

    NetStat getNetStat() throws SysmException;

    NetStat getNetStat(byte[] address, long port) throws SysmException;

    NetRoute[] getNetRoutes() throws SysmException;

    NetInterfaceConfig getNetInterfaceConfig() throws SysmException;

    NetInterfaceConfig getNetInterfaceConfig(String ifName) throws SysmException;

    NetInterfaceStat getNetInterfaceStat(String ifName) throws SysmException;

    String[] getNetInterfaces() throws SysmException;

    NetConnection[] getNetConnections(int flags) throws SysmException;

    String getNetListenAddress(long port) throws SysmException;

    Who[] getWhos() throws SysmException;
}
