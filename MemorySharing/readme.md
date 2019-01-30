Native对象和 Java对象的对应关系

内存共享机制

你去阅读opencv的时候看到下面的代码
~~~
public final long nativeObj;

    public Mat(long addr)
    {
        if (addr == 0)
            throw new java.lang.UnsupportedOperationException("Native object address is NULL");
        nativeObj = addr;
    }
~~~