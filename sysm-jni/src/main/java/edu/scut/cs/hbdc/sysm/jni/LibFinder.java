package edu.scut.cs.hbdc.sysm.jni;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import java.io.File;
import java.io.IOException;
import java.net.URL;
import java.net.URLClassLoader;
import java.nio.file.Files;
import java.nio.file.LinkOption;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.util.List;
import java.util.Objects;
import java.util.StringTokenizer;
import java.util.stream.Collectors;

public final class LibFinder extends Finder{
    private static final Logger log = LoggerFactory.getLogger(LibFinder.class);
    private LibFinder() {}

    public static Path find(String path, String libName) throws ArchLoaderException, IOException {
        if (libName == null || "".equals(libName)) {
            throw new IllegalArgumentException("The libName is null or empty");
        }
        if (path == null || "".equals(path)) {
            return null;
        }
        Path absPath = Paths.get(path).toAbsolutePath();

        return search(absPath, libName, 1);
    }

    /**
     * Find if the libName along side with jars in classpath
     */
    public static Path findBesideJar(String libName,
                                     ClassLoader loader,
                                     String resPath,
                                     StringBuilder jarName) throws ArchLoaderException, IOException {
        if (libName == null || "".equals(libName)) {
            throw new IllegalArgumentException("The libName is null or empty");
        }
        Objects.requireNonNull(loader);

        Path lib = null;
        //get resPath under the classpath root
        //e.g. jar:file:/xx/xx
        URL url = loader.getResource(resPath);
        //e.g. /home/user/.m2/repo/edu/scut/cs/sysm.jar!/edu/scut/cs/sysm
        Path jarPath = null;

        if (url != null && JarFinder.isJarUrl(url, jarName)) {
            jarPath = url2Path(url);
        }

        if (url == null) {
            jarPath = JarFinder.findJarPath(loader, jarName);
        }

        while (jarPath != null
                && jarPath.toString().contains(".jar")) {
            jarPath = jarPath.getParent();
        }

        if (jarPath != null) {
           lib = search(jarPath, libName, 1);
        }

        return lib;
    }



    /**
     * Find if the libName in ${classpath}
     */
    public static Path findInClasspath(String libName,
                                       ClassLoader loader,
                                       String resPath) throws ArchLoaderException, IOException {
        if (libName == null || "".equals(libName)) {
            throw new IllegalArgumentException("The libName is null or empty");
        }
        Objects.requireNonNull(loader);

        Path lib = null;
        //get resPath under the classpath root
        URL url = loader.getResource(resPath);
        Path classpath;

        if (url != null && url.getProtocol().equals("file")) {
            classpath = Paths.get(url.getPath());
            lib = search(classpath, libName, 1);
        }

        if (lib == null && loader instanceof URLClassLoader) {
            URL[] urls = ((URLClassLoader)loader).getURLs();
            //e.g. file:/xx/xx/xx.jar
            for (URL tmp: urls) {
                classpath = Paths.get(tmp.getPath())
                                 .getParent();
                lib = search(classpath, libName, 1);
            }
        }

        if (lib == null
                && (url = loader.getResource("")) != null) {
                classpath = Paths.get(url.getPath());
                lib = search(classpath, libName, 1);
        }

        return lib;
    }

    /**
     * Find if the libName along side with ${classpath}
     */
    public static Path findBesideClasspath(String libName,
                                           ClassLoader loader) throws ArchLoaderException, IOException {
        if (libName == null || "".equals(libName)) {
            throw new IllegalArgumentException("The libName is null or empty");
        }
        Objects.requireNonNull(loader);

        Path lib = null;
        URL url = loader.getResource("");
        Path classpath;

        if (url != null) {
            classpath = Paths.get(url.getPath());
            if (Files.exists(classpath)
                    && Files.isDirectory(classpath)) {
                classpath = classpath.getParent();
                if (classpath != null) {
                    lib = search(classpath, libName, 1);
                }

                if (lib == null) {
                    lib = search(classpath, libName, 2);
                }
            }
        }

        if (lib == null && loader instanceof URLClassLoader) {
            URL[] urls = ((URLClassLoader)loader).getURLs();
            //e.g. file:/xx/xx/xx.jar
            for (URL tmp: urls) {
                classpath = Paths.get(tmp.getPath())
                        .getParent();
                if (classpath != null) {
                    lib = search(classpath, libName, 1);
                }

                if (lib == null) {
                    lib = search(classpath, libName, 2);
                }
            }
        }

        return lib;
    }

    public static Path findInJavaLibPath(String libName) throws ArchLoaderException, IOException {
        Path lib = null;
        String path = System.getProperty("java.library.path", "");
        StringTokenizer tok =
                new StringTokenizer(path, File.pathSeparator);
        while (tok.hasMoreTokens()) {
            path = tok.nextToken();
            lib = search(Paths.get(path), libName, 1);
            if (lib != null) {
                break;
            }
        }
        return lib;
    }

    private static Path search(Path root, final String libName, int maxDepth) throws ArchLoaderException, IOException {
        if (root == null
                || !Files.exists(root, LinkOption.NOFOLLOW_LINKS)
                || !Files.isDirectory(root, LinkOption.NOFOLLOW_LINKS)) {
            if (log.isDebugEnabled()) {
                log.warn("path "+ root +" isn't existed or a directory");
            }
            return null;
        }

        try {
            List<Path> res = Files.walk(root, maxDepth)
                    .filter(path -> path.getFileName().endsWith(libName))
                    .collect(Collectors.toList());
            if (!res.isEmpty()) {
                if (res.size() == 1) {
                    return res.get(0).toAbsolutePath();
                }
                throw new ArchLoaderException("There is duplicate native lib " + libName);
            }
        } catch (IOException e) {
            String msg = "fail to search " + libName + " in " + root;
            log.error(msg, e);
            throw new IOException(msg);
        }
        return null;
    }
}
