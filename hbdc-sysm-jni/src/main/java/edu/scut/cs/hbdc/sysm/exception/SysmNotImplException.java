package edu.scut.cs.hbdc.sysm.exception;

public class SysmNotImplException extends SysmException {

    private static final String msg =
            "This method has not been implemented yet";

    public static final SysmNotImplException INSTANCE =
            new SysmNotImplException(msg);

    public SysmNotImplException() {
    }

    public SysmNotImplException(String s) {
        super(s);
    }
}
