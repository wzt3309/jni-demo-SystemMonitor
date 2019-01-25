package edu.scut.cs.hbdc.sysm.exception;

import edu.scut.cs.hbdc.sysm.SysmProxy;

public class SysmPermissionDeniedException extends SysmException {

    public SysmPermissionDeniedException(String s) {
        super(s);
    }

    public SysmPermissionDeniedException(SysmProxy sysm, long pid) throws SysmException {
        super(getUserDeniedMsg(sysm, pid));
    }

    public static String getUserDeniedMsg(SysmProxy sysm, long pid) throws SysmException {
        String user = "unknown", owner = "unknown";

        user = sysm.getProcessOwner(sysm.getPid()).getUser();
        owner = sysm.getProcessOwner(pid).getUser();

        return
                "User " + user + " denied access to process " +
                pid + " owned by " + owner;
    }
}
