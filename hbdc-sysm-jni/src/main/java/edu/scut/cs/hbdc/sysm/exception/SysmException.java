package edu.scut.cs.hbdc.sysm.exception;

public class SysmException extends Exception {
    private String message;

    public SysmException () { super(); }

    public SysmException (String s) { super(s); }

    public String getMessage() {
        if (this.message != null) {
            return this.message;
        }
        else {
            return super.getMessage();
        }
    }

    void setMessage(String message) {
        this.message = message;
    }
}
