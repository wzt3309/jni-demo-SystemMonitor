package edu.scut.cs.hbdc.sysm.bean;

import edu.scut.cs.hbdc.sysm.Sysm;

import java.io.Serializable;

public class CpuPercentage extends AbstractBean implements Serializable{

    private static final long serialVersionUID = 1801343789274322897L;
    private double user;
    private double sys;
    private double nice;
    private double idle;
    private double wait;
    private double hi;
    private double si;
    private double steal;
    private double combined;

    public CpuPercentage() {
    }


    public native void gather(Sysm sysm, Cpu oldCpu, Cpu curCpu);

    public static CpuPercentage fetch(Sysm sysm, Cpu oldCpu, Cpu curCpu) {
        CpuPercentage perc = new CpuPercentage();
        perc.gather(sysm, oldCpu, curCpu);
        return perc;
    }
    /**
     * Get the User Cpu Percentage
     * @return User Cpu Percentage
     */
    public double getUser() {
        return user;
    }

    /**
     * Get the Sys Cpu Percentage
     * @return Sys Cpu Percentage
     */
    public double getSys() {
        return sys;
    }

    /**
     * Get the Nice Cpu Percentage
     * @return Nice Cpu Percentage
     */
    public double getNice() {
        return nice;
    }

    /**
     * Get the Idle Cpu Percentage
     * @return Idle Cpu Percentage
     */
    public double getIdle() {
        return idle;
    }

    /**
     * Get the Wait Cpu Percentage
     * @return Wait Cpu Percentage
     */
    public double getWait() {
        return wait;
    }

    /**
     * Get the Hi Cpu Percentage
     * @return Hi Cpu Percentage
     */
    public double getHi() {
        return hi;
    }

    /**
     * Get the Si Cpu Percentage
     * @return Si Cpu Percentage
     */
    public double getSi() {
        return si;
    }

    /**
     * Get the Steal Cpu Percentage
     * @return Steal Cpu Percentage
     */
    public double getSteal() {
        return steal;
    }

    /**
     * Sum of User + Sys + Nice + Wait
     * @return Sum of User + Sys + Nice + Wait
     */
    public double getCombined() {
        return combined;
    }

    @Override
    public String toString() {
        return "CPU states: " +
                format(this.user) + " us, " +
                format(this.sys) + " sy, " +
                format(this.nice) + " ni, " +
                format(this.idle) + " id, " +
                format(this.wait) + " wa, " +
                format(this.hi) + " hi, " +
                format(this.si) + " si, " +
                format(this.steal) + " st, " +
                format(this.combined) + " co";

    }
}
