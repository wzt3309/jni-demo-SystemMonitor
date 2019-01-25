package edu.scut.cs.hbdc.sysm.jni;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import java.io.IOException;
import java.nio.file.Path;

import static edu.scut.cs.hbdc.sysm.jni.LibFinder.*;

public class ArchLoader {
    private final static Logger log = LoggerFactory.getLogger(ArchLoader.class);

    private final Object loadLock = new Object();
    private boolean loaded = false;

    private String name;
    private String packageName;
    private String jarName;
    private String libName;
    private String resourcePath;
    private Class loaderClass;
    private Path nativeLibrary;
    private String version;

    public ArchLoader() {
    }

    public ArchLoader(Class loaderClass) {
        if (loaderClass == null) {
            throw new IllegalArgumentException("The loaderCLass can't be null");
        }
        setLoaderClass(loaderClass);

        //e.g. Sysm
        String var1 = loaderClass.getName();

        //e.g. edu.scut.cs.sysm
        String packageName = var1.substring(0, var1.lastIndexOf("."));
        setPackageName(packageName);

        //e.g. sysm
        String name = packageName.substring(packageName.lastIndexOf(".") + 1);
        setName(name);

        //e.g. sysm.jar
        setJarName(getName() + ".jar");

        //e.g. edu/scut/cs/sysm
        setResourcePath(packageName.replaceAll("\\.", "/"));
    }

    public void load(String path)
            throws ArchNotSupportedException, ArchLoaderException {
        synchronized (loadLock) {
            if (!loaded) {
                loadLib(path);
                loaded = true;
            }
        }
    }

    protected void loadLib(String path)
            throws ArchNotSupportedException, ArchLoaderException {
        try {
            String libName = getLibName();
            String versionedLibName = getVersionedLibName();

            if (path == null || "".equals(path)) {
                path = System.getProperty(getPackageName() + ".path");
            }

            if (path != null) {
                //"-" means the native library is already loaded
                if ("-".equals(path)) return;

                nativeLibrary = find(path, libName);
                if (nativeLibrary == null && versionedLibName != null) {
                    nativeLibrary = find(path, versionedLibName);
                }
            } else {
                StringBuilder jarName = new StringBuilder(getJarName());

                nativeLibrary = findBesideJar(libName, getClassLoader(), getResourcePath(), jarName);
                if (nativeLibrary == null && versionedLibName != null) {
                    nativeLibrary = findBesideJar(versionedLibName, getClassLoader(), getResourcePath(), jarName);
                }
                setJarName(jarName.toString());
                if (nativeLibrary == null) {
                    nativeLibrary = findInClasspath(libName, getClassLoader(), getResourcePath());
                }
                if (nativeLibrary == null && versionedLibName != null) {
                    nativeLibrary = findInClasspath(versionedLibName, getClassLoader(), getResourcePath());
                }
                if (nativeLibrary == null) {
                    nativeLibrary = findBesideClasspath(libName, getClassLoader());
                }
                if (nativeLibrary == null && versionedLibName != null) {
                    nativeLibrary = findBesideClasspath(versionedLibName, getClassLoader());
                }
                if (nativeLibrary == null) {
                    nativeLibrary = findInJavaLibPath(libName);
                }
                if (nativeLibrary == null && versionedLibName != null) {
                    nativeLibrary = findInJavaLibPath(versionedLibName);
                }
            }

            if (nativeLibrary != null) {
                systemLoad(nativeLibrary.toString());
            } else {
                //LD_LIBRARY_PATH must be set for linux
                systemLoadLibrary(libName);
            }
        } catch (RuntimeException | IOException e) {
            String msg = "Fail to load " + libName;
            log.error(msg, e);
            throw new ArchLoaderException(msg);
        }

    }

    /**
     * Get full name of native library
     * e.g. libsysm-amd64-linux.so
     * @return Full name of native library
     * @throws ArchNotSupportedException Some operating system
     * platform isn't supported (just support linux now)
     */
    public String getLibName() throws ArchNotSupportedException {
        ensureLibName();
        String prefix = getLibPrefix();
        String ext = getLibExtension();

        return prefix + libName + ext;
    }

    public void setLibName(String libName) {
        this.libName = libName;
    }

    /**
     * Get full name with version of native library
     * e.g. libsysm-amd64-linux-1.0.0.so
     * @return Full name with version of native library
     * @throws ArchNotSupportedException Some operating system
     * platform isn't supported (just support linux now)
     */
    public String getVersionedLibName() throws ArchNotSupportedException {
        ensureLibName();
        String prefix = getLibPrefix();
        String ext = getLibExtension();
        if (version == null || "".equals(version)) {
            if (log.isDebugEnabled())
                log.warn("Version is not specified");
            return null;
        }
        return prefix + libName + "-" + version + ext;
    }

    /**
     * Format and get the native Library name with operating system architecture name
     * e.g. sysm-amd64-linux
     * @return Native Library name with operating system architecture name
     * @throws ArchNotSupportedException Some operating system
     * platform isn't supported (just support linux now)
     */
    protected String getArchLibName() throws ArchNotSupportedException{
       return getName() + "-" + ArchName.getName();
    }

    /**
     * Get default native library name from System.getProperty(getPackageName() + ".libname")
     * If the value is null, then use the default value "hbdc" + getArchLibName()
     * e.g. hbdcsysm-amd64-linux
     * @return Default native library name
     * @throws ArchNotSupportedException Some operating system
     * platform isn't supported (just support linux now)
     */
    protected String getDefaultLibName() throws ArchNotSupportedException {
        return System.getProperty(getPackageName() + ".lib.name",
                "hbdc" + getArchLibName());
    }

    protected void ensureLibName() throws ArchNotSupportedException {
        if (libName == null || "".equals(libName)) {
            libName = getDefaultLibName();
        }
    }

    protected static String getLibPrefix() {
        return "lib";
    }

    protected static String getLibExtension() {
        return ".so";
    }

    protected void systemLoad(String libAbsPath) {
        System.load(libAbsPath);
    }

    protected void systemLoadLibrary(String libName) {
        System.loadLibrary(libName);
    }

    public Class getLoaderClass() {
        return loaderClass;
    }

    public void setLoaderClass(Class loaderClass) {
        this.loaderClass = loaderClass;
    }

    public ClassLoader getClassLoader() {
        return loaderClass != null ? loaderClass.getClassLoader() : null;
    }

    public String getName() {
        return name;
    }

    public void setName(String name) {
        this.name = name;
    }

    public String getPackageName() {
        return packageName;
    }

    public void setPackageName(String packageName) {
        this.packageName = packageName;
    }

    public String getJarName() {
        return jarName;
    }

    public void setJarName(String jarName) {
        this.jarName = jarName;
    }

    public String getResourcePath() {
        return resourcePath;
    }

    public void setResourcePath(String resourcePath) {
        this.resourcePath = resourcePath;
    }

    public Path getNativeLibrary() {
        return nativeLibrary;
    }

    public String getVersion() {
        return version;
    }

    public void setVersion(String version) {
        this.version = version;
    }
}
