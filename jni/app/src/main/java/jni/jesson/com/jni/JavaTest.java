package jni.jesson.com.jni;

import android.util.Log;
import android.widget.Toast;

class JavaTest {
    public native void testeeew();
    public native void printMethod();

    public native void cDiaojavaJingTai();

    public native void cDiaojava();

    public native byte[] returnAByteArray();

    public native String stringManipulator(String s, String[] s1);

    public JavaTest() {
        System.loadLibrary("native-lib");
    }


    /**
     * 让c通过new对象调用的非静态方法对应
     *
     */
    public void cNewObjDJ(){
        Log.i("myDemo-jni","c通过new对象的方式调用函数");
    }


    public static void cDiao(String str){
        Log.i("myDemo-jni","c调用java静态方法" + str);
    }


    public int number = 9;
    public String content = "I'm from Java!";

    /**
     * 通过c来修改Java 的属性
     */
    public native void changeValue();

    /**
     * jni 动态注册
     * @param i
     * @return
     */
    public native int dynamicJavaTest(int i);


    /**
     * jni 异常
     */
    public native void doit() throws Exception;

    public static void exceptionCallback() {
        int a = 20 / 0;
        System.out.println("--->" + a);
    }

    public static void normalCallback() {
        System.out.println("In Java: invoke normalCallback.");
    }

    /**
     * jni 线程
     */
    //由JNI中的线程回调
     private static void fromJNI(int i) {
         Log.v("Java------>", ""+i);
     }

     //本地方法
     private native void mainThread();
     private native void setJNIEnv();
     private native void setJNIEnv444();



}