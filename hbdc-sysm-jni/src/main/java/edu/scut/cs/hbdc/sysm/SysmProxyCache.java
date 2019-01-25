package edu.scut.cs.hbdc.sysm;

import edu.scut.cs.hbdc.sysm.exception.SysmException;
import edu.scut.cs.hbdc.sysm.exception.SysmNotImplException;
import edu.scut.cs.hbdc.sysm.exception.SysmPermissionDeniedException;

import java.lang.reflect.InvocationHandler;
import java.lang.reflect.InvocationTargetException;
import java.lang.reflect.Method;
import java.lang.reflect.Proxy;
import java.util.Map;

public class SysmProxyCache implements InvocationHandler{

    private Sysm sysm;
    private Map cache = ReferenceMap.newInstance();
    public static final int EXPIRE_DEFAULT = 30 * 1000; //30s
    private int expire;

    private SysmProxyCache(Sysm sysm, int expire) {
        this.sysm = sysm;
        this.expire = expire;
    }

    public static SysmProxy newInstance() {
        return newInstance(Sysm.getInstance());
    }

    public static SysmProxy newInstance(Sysm sysm) {
        return newInstance(sysm, EXPIRE_DEFAULT);
    }

    public static SysmProxy newInstance(Sysm sysm, int expire) {
        SysmProxyCache handler = new SysmProxyCache(sysm, expire);
        SysmProxy proxy;

        proxy = (SysmProxy) Proxy.newProxyInstance(SysmProxy.class.getClassLoader(),
                                                   new Class[]{SysmProxy.class},
                                                   handler);

        return proxy;
    }

    private static SysmProxyCache getHandler(Object proxy) {
        return (SysmProxyCache)Proxy.getInvocationHandler(proxy);
    }

    public static void clear(Object proxy) {
        getHandler(proxy).cache.clear();
    }

    public static Sysm getSysm(Object proxy) {
        if (proxy.getClass() == Sysm.class) {
            return (Sysm)proxy;
        } else {
            return getHandler(proxy).sysm;
        }
    }

    @Override
    public Object invoke(Object proxy, Method method, Object[] args) throws Throwable {
        SysmCacheObject cacheVal = null;
        Object retval;
        Object argKey = null;   // args to invode method
        Map argMap = null;      // map to keep value which reference to specified args
        long timeNow = System.currentTimeMillis();

        if (args != null) {
            if (args.length == 1) {
                argKey = args[0];
            } else {
                int hashCode = 0;
                for (int i = 0;i < args.length;i++) {
                    hashCode ^= args[i].hashCode();
                }
                argKey = hashCode;
            }

            argMap = (Map) this.cache.get(method);
            if (argMap == null) {
                argMap = ReferenceMap.newInstance();
            } else {
                cacheVal = (SysmCacheObject) argMap.get(argKey);
            }
        } else {
            cacheVal = (SysmCacheObject) this.cache.get(method);
        }

        if (cacheVal == null) {
            cacheVal = new SysmCacheObject();
        }

        if (cacheVal.value != null) {
            // cache out of time
            if ((timeNow - cacheVal.timestamp) > this.expire) {
                cacheVal.value = null;
            }
        }

        if (cacheVal.value == null) {
            try {
                retval = method.invoke(this.sysm, args);
            } catch (InvocationTargetException e) {
                Throwable t = e.getTargetException();

                String msg;

                if (t instanceof SysmException) {
                    msg = "";
                } else {
                    msg = t.getClass().getName() + ": ";
                }

                msg += t.getMessage();

                if (argKey != null) {
                    msg += ": " + getDebugArgs(args, argKey);
                }

                if (t instanceof SysmNotImplException) {
                    throw new SysmNotImplException(msg);
                } else if (t instanceof SysmPermissionDeniedException) {
                    throw new SysmPermissionDeniedException(msg);
                }

                throw new SysmException(msg);
            } catch (Exception e){
                String msg =
                        e.getClass().getName() + ": " +
                                e.getMessage();

                if (argKey != null) {
                    msg += ": " + getDebugArgs(args, argKey);
                }

                throw new SysmException(msg);
            }

            cacheVal.value = retval;
            cacheVal.timestamp = timeNow;

            if (args == null) {
                this.cache.put(method, cacheVal);
            } else {
                argMap.put(argKey, cacheVal);
                this.cache.put(method, argMap);
            }
        } else {
            retval = cacheVal.value;
        }

        return retval;
    }

    private String getDebugArgs(Object[] args, Object argKey) {

        if (args.length == 0) {
            return null;
        }

        StringBuffer dargs =
                new StringBuffer(args[0].toString());

        for (int i=1; i<args.length; i++) {
            dargs.append(',').append(args[i].toString());
        }

        if (!dargs.toString().equals(argKey.toString())) {
            dargs.append('/').append(argKey);
        }

        return dargs.toString();
    }
}
