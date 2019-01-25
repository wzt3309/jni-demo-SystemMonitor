package edu.scut.cs.hbdc.sysm.jni;

import java.net.URL;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;

public abstract class Finder {
    public static Path url2Path(URL url) {
        if (url == null) {
            return null;
        }
        Path res;
        String urlName = url.getPath();
        int ix = 0;
        if ((ix = urlName.indexOf("file:")) != -1) {
            urlName = urlName.substring(ix + 5);
        }

        res = Paths.get(urlName);

        return res;
    }
}
