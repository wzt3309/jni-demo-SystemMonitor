package edu.scut.cs.hbdc.sysm.bean;

import edu.scut.cs.hbdc.sysm.exception.SysmException;
import edu.scut.cs.hbdc.sysm.Sysm;

import java.io.Serializable;

public class ResourceLimit extends AbstractBean implements Serializable{

    private static final long serialVersionUID = -666385672071329561L;
    private long cpuCur;
    private long cpuMax;
    private long fileSizeCur;
    private long fileSizeMax;
    private long pipeSizeCur;
    private long pipeSizeMax;
    private long dataCur;
    private long dataMax;
    private long stackCur;
    private long stackMax;
    private long coreCur;
    private long coreMax;
    private long memoryCur;
    private long memoryMax;
    private long processesCur;
    private long processesMax;
    private long openFilesCur;
    private long OpenFilesMax;
    private long virtualMemoryCur;
    private long virtualMemoryMax;

    public ResourceLimit() {
    }

    public native void gather(Sysm sysm) throws SysmException;

    public static ResourceLimit fetch(Sysm sysm) throws SysmException {
        ResourceLimit resourceLimit = new ResourceLimit();
        resourceLimit.gather(sysm);
        return resourceLimit;
    }

    /**
     * The maximum amount of CPU time in seconds that can be used by a process.
     * <sys/resource.h> RLIMIT_CPU
     * @return cpu_cur soft limit
     */
    public long getCpuCur() {
        return cpuCur;
    }

    /**
     * The maximum amount of CPU time in seconds that can be used by a process.
     * <sys/resource.h> RLIMIT_CPU
     * @return cpu_cur hard limit
     */
    public long getCpuMax() {
        return cpuMax;
    }

    /**
     * The maximum size of a file in bytes that can be created by a process.
     * <sys/resource.h> RLIMIT_FSIZE
     * @return file_size_cur soft limit
     */
    public long getFileSizeCur() {
        return fileSizeCur;
    }

    /**
     * The maximum size of a file in bytes that can be created by a process.
     * <sys/resource.h> RLIMIT_FSIZE
     * @return file_size_cur hard limit
     */
    public long getFileSizeMax() {
        return fileSizeMax;
    }

    /**
     * The maximum size of a pipe in bytes that can be created by a process.
     * <limits.h> PIPE_BUF
     * @return pipe_size_cur soft limit
     */
    public long getPipeSizeCur() {
        return pipeSizeCur;
    }

    /**
     * The maximum size of a pipe in bytes that can be created by a process.
     * <limits.h> PIPE_BUF
     * @return pipe_size_cur hard limit
     */
    public long getPipeSizeMax() {
        return pipeSizeMax;
    }

    /**
     * This is the maximum size of the process's data segment (initialized
     * data, uninitialized data, and heap).
     * <sys/resource.h> RLIMIT_DATA
     * @return data_cur soft limit
     */
    public long getDataCur() {
        return dataCur;
    }

    /**
     * This is the maximum size of the process's data segment (initialized
     * data, uninitialized data, and heap).
     * <sys/resource.h> RLIMIT_DATA
     * @return data_cur hard limit
     */
    public long getDataMax() {
        return dataMax;
    }

    /**
     * This is the maximum size of the process stack, in bytes.
     * <sys/resource.h> RLIMIT_STACK
     * @return stack_cur soft limit
     */
    public long getStackCur() {
        return stackCur;
    }

    /**
     * This is the maximum size of the process stack, in bytes.
     * <sys/resource.h> RLIMIT_STACK
     * @return stack_cur hard limit
     */
    public long getStackMax() {
        return stackMax;
    }

    /**
     * This is the maximum size of a core file in bytes that the
     * process may dump.
     * <sys/resource.h> RLIMIT_CORE
     * @return core_cur soft limit
     */
    public long getCoreCur() {
        return coreCur;
    }

    /**
     * This is the maximum size of a core file in bytes that the
     * process may dump.
     * <sys/resource.h> RLIMIT_CORE
     * @return core_cur hard limit
     */
    public long getCoreMax() {
        return coreMax;
    }

    /**
     * This is a limit (in bytes) on the process's resident set (the
     * number of virtual pages resident in RAM).
     * <sys/resource.h> RLIMIT_RSS
     * @return memory_cur soft limit
     */
    public long getMemoryCur() {
        return memoryCur;
    }

    /**
     * This is a limit (in bytes) on the process's resident set (the
     * number of virtual pages resident in RAM).
     * <sys/resource.h> RLIMIT_RSS
     * @return memory_cur hard limit
     */
    public long getMemoryMax() {
        return memoryMax;
    }

    /**
     * This is the maximum number of processes (or, more precisely on
     * Linux, threads) that can be created for the real user ID of
     * the calling process.
     * <sys/resource.h> RLIMIT_NPROC
     * @return processes_cur soft limit
     */
    public long getProcessesCur() {
        return processesCur;
    }

    /**
     * This is the maximum number of processes (or, more precisely on
     * Linux, threads) that can be created for the real user ID of
     * the calling process.
     * <sys/resource.h> RLIMIT_NPROC
     * @return processes_cur hard limit
     */
    public long getProcessesMax() {
        return processesMax;
    }

    /**
     * This specifies a value one greater than the maximum file
     * descriptor number that can be opened by this process.
     * <sys/resource.h> RLIMIT_NOFILE
     * @return open_files_cur soft limit
     */
    public long getOpenFilesCur() {
        return openFilesCur;
    }

    /**
     * This specifies a value one greater than the maximum file
     * descriptor number that can be opened by this process.
     * <sys/resource.h> RLIMIT_NOFILE
     * @return open_files_cur hard limit
     */
    public long getOpenFilesMax() {
        return OpenFilesMax;
    }

    /**
     * This is the maximum size of the process's virtual memory
     * (address space).
     * <sys/resource.h> RLIMIT_AS
     * @return virtual_memory_cur soft limit
     */
    public long getVirtualMemoryCur() {
        return virtualMemoryCur;
    }

    /**
     * This is the maximum size of the process's virtual memory
     * (address space).
     * <sys/resource.h> RLIMIT_AS
     * @return virtual_memory_cur hard limit
     */
    public long getVirtualMemoryMax() {
        return virtualMemoryMax;
    }
}
