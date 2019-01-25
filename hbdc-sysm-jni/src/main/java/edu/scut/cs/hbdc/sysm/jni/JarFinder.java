package edu.scut.cs.hbdc.sysm.jni;

import java.net.URL;
import java.net.URLClassLoader;
import java.nio.file.Path;
import java.util.Objects;

public final class JarFinder extends Finder{

    private JarFinder() {
    }

    public static Path findJarPath(ClassLoader loader, StringBuilder jarName) {
        Objects.requireNonNull(loader);

        Path jarPath = null;
        URL url = null;

        if (loader instanceof URLClassLoader) {
            //get other url under the root to find the jar
            URL[] urls = ((URLClassLoader)loader).getURLs();
            //e.g. file:/xx/xx/xx
            for (URL tmp: urls) {
                if (isJarUrl(tmp, jarName)) {
                    url = tmp;
                }
            }
        }

        if (url != null) {
            jarPath = url2Path(url);
        }

        return jarPath;
    }

    public static boolean isJarUrl(URL url, StringBuilder jarName) {
        if (url == null
                || jarName == null
                || jarName.length() == 0
                || !jarName.toString().endsWith(".jar")) {
            return false;
        }

        String path = url.getPath();
        //jar maybe versioned
        if (!path.contains(jarName)) {
            jarName.delete(jarName.indexOf(".jar"), jarName.length());
            jarName.append("-");
            int i = path.indexOf(jarName.toString());
            if (i != -1) {
                int j = jarName.length();
                j += i;
                i = path.lastIndexOf(".jar");
                if (i != -1) {
                    i += 4;
                    jarName.append(path.substring(j, i));
                }
            }
        }

        return path.contains(jarName);
    }
}
