package edu.scut.cs.hbdc.sysm.bean;

import edu.scut.cs.hbdc.sysm.query.ProcessFinder;
import edu.scut.cs.hbdc.sysm.Sysm;

import java.io.Serializable;
import java.util.HashMap;
import java.util.Map;

/**
 * Gather the state of multiple process matched by a query
 * For example: query = "State.Name.eq=echo \"Test\"" will
 * find all process with the the Name of the process program
 * (ProcessState.name) is "echo \"Test\""
 */
public class MultiProcessCpu extends ProcessCpu implements Serializable {

    private static final long serialVersionUID = -713351755558876178L;
    /**
     * A hash map to store the monitor cpu of a set of processes
     * key = query
     * value = this
     */
    private static Map<String, MultiProcessCpu> qtable = new HashMap<>();

    /**
     * Set a fictitious 'pid' for a set of processes
     */
    private long pid;
    /**
     * The number of the set of processes matched by query
     */
    private int pnum;

    /**
     * Use a query to find a set of processes and get them total
     * use of cpu
     * @param sysm Sysm class
     * @param query the query to find a set of processes
     * @return MultiProcessCpu
     */
    static synchronized MultiProcessCpu get(Sysm sysm, String query) {
        MultiProcessCpu mpCpu;

        mpCpu = qtable.get(query);

        if (mpCpu == null) {
            mpCpu = new MultiProcessCpu();
            mpCpu.pid = query.hashCode();
            qtable.put(query, mpCpu);
        }

        long now = System.currentTimeMillis();
        double diff = now - mpCpu.getLastTime();
        if (diff == 0) {
            return mpCpu;
        }

        long oldTotal = mpCpu.getTotal();

        mpCpu.setTotal(0);
        mpCpu.setUser(0);
        mpCpu.setSys(0);
        mpCpu.pnum = 0;

        long[] pids = ProcessFinder.find(sysm, query);
        if (pids != null) {
            mpCpu.pnum = pids.length;
            for (long pid: pids) {
                //TODO
                ProcessTime pTime = new ProcessTime();
                mpCpu.setTotal(mpCpu.getTotal() + pTime.getTotal());
                mpCpu.setUser(mpCpu.getUser() + pTime.getUser());
                mpCpu.setSys(mpCpu.getSys() + pTime.getSys());
            }

            // In order to calculate percent, must be call first
            if (oldTotal == 0) {
                return mpCpu;
            }

            // the cpu time using by the set of processes in a period of time
            double percent = (mpCpu.getTotal() - oldTotal) / diff;

            if (percent < 0.0) {
                percent = 0.0 - percent;
            }
            if (percent >= 1.0) {
                percent = 0.99;
            }
            mpCpu.setPercent(percent);
        }

        return mpCpu;
    }

    /**
     * A fictitious 'pid' for a set of processes matched by query
     * @return 'pid' for a set of processes matched by query
     */
    public long getPid() {
        return pid;
    }

    /**
     * The number of the set of processes matched by query
     * @return Number of the set of processes matched by query
     */
    public int getPnum() {
        return pnum;
    }

    @Override
    public int hashCode() {
        return (int)this.pid;
    }

    @Override
    public boolean equals(Object obj) {
        if (this == obj) return true;
        if (obj == null || !(obj instanceof MultiProcessCpu)) return false;

        return ((MultiProcessCpu)obj).pid == this.pid;
    }
}
