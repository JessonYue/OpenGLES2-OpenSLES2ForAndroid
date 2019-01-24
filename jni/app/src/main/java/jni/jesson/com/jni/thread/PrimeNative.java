package jni.jesson.com.jni.thread;

import android.annotation.SuppressLint;
import android.os.Handler;
import android.os.Message;

import jni.jesson.com.jni.MainActivity;

public class PrimeNative {
    private int start;
    private int end;

    private static Handler h;
    private static String thName;


    @SuppressLint("HandlerLeak")
    public PrimeNative(int start, int end) {
        this.start = start;
        this.end = end;
        if (start > 0 && start < end) {
            init(start, end); // 将需要计算的数据传递C层.
        }
        h = new Handler() {
            public void handleMessage(Message msg) {
                MainActivity.counterView.append(msg.arg1+", ");
            }
        };
    }

    void callBack(int result) {
        Message m = h.obtainMessage(1, result, 3, null);
        thName = Thread.currentThread().getName();
        h.sendMessage(m);
    }

    private native void init(int start, int end);

}
