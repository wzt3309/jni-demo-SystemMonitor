package edu.scut.cs.hbdc.sysm;

import java.lang.ref.Reference;
import java.lang.ref.ReferenceQueue;
import java.lang.ref.SoftReference;
import java.lang.ref.WeakReference;
import java.util.*;

public class ReferenceMap extends AbstractMap {

    protected ReferenceQueue queue;
    protected Map map;

    private ReferenceMap() {
        this(new HashMap<>());
    }

    private ReferenceMap(Map map) {
        this.map = map;
        this.queue = new ReferenceQueue();
    }

    public static Map synchronizedMap() {
        Map map = Collections.synchronizedMap(new HashMap<>());
        return newInstance(map);
    }

    public static Map newInstance() {
        return newInstance(new HashMap<>());
    }

    public static Map newInstance(Map map) {
        return new ReferenceMap(map);
    }

    @Override
    public Object get(Object key) {
        Reference ref = (Reference) this.map.get(key);
        if (ref == null) {
            return null;
        } else {
            Object o = ref.get();
            if (o == null) {
                this.map.remove(key);
            }
            return o;
        }
    }

    @Override
    public Object put(Object key, Object value) {
        poll();
        return this.map.put(key, new SoftValue(key, value, queue));
    }

    @Override
    public Object remove(Object key) {
        poll();
        return this.map.remove(key);
    }

    @Override
    public void clear() {
        poll();
        this.map.clear();
    }

    @Override
    public int size() {
        poll();
        return this.map.size();
    }

    @Override
    public Set entrySet() {
        throw new UnsupportedOperationException();
    }


    protected void poll() {
        MapReference ref;

        while ((ref = (MapReference)this.queue.poll()) != null) {
            this.map.remove(ref.getKey());
        }
    }

    protected interface MapReference {
        Object getKey();
    }

    protected static final class SoftValue
            extends SoftReference implements MapReference {
        private Object key;

        private SoftValue(Object key, Object value, ReferenceQueue queue) {
            super(value, queue);
            this.key = key;
        }

        @Override
        public Object getKey() {
            return this.key;
        }
    }

    protected static final class WeakValue extends WeakReference implements MapReference {
        private Object key;

        protected WeakValue(Object key, Object value, ReferenceQueue queue) {
            super(value, queue);
            this.key = key;
        }

        @Override
        public Object getKey() {
            return this.key;
        }
    }

}
