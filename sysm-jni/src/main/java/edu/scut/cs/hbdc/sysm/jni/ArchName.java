package edu.scut.cs.hbdc.sysm.jni;

public final class ArchName {
    private ArchName () { }

    static boolean useDmalloc =
            System.getProperty("jni.dmalloc") != null;

    public static String getName() throws ArchNotSupportedException {
        String name = getArchName();
        if (useDmalloc) {
            name += "-dmalloc";
        }
        return name;
    }

    private static boolean is64() {
        return "64".equals(System.getProperty("sun.arch.data.model"));
    }

    private static String getArchName() throws ArchNotSupportedException {
        String name    = System.getProperty("os.name");
        String arch    = System.getProperty("os.arch");
        String version = System.getProperty("os.version");

        if (arch.endsWith("86")) {
            arch = "x86";
        }

        if (name.equals("Linux")) {
            return arch + "-linux";
        }

        String desc = arch + "-" + name + "-" + version;
        throw new ArchNotSupportedException("platform (" + desc + ") not supported");
    }

}
