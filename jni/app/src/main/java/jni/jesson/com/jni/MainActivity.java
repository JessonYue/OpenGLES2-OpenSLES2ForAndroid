package jni.jesson.com.jni;

import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.widget.TextView;

import jni.jesson.com.jni.thread.PrimeNative;

public class MainActivity extends AppCompatActivity {

    JavaTest javatest;
    private PrimeNative pn;
    public static MainActivity ref;
    public static TextView counterView;



    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        counterView = findViewById(R.id.sample_text);
        javatest = new JavaTest();
        pn = new PrimeNative(10, 100);
        ref = this;

        javatest.printMethod();
        byte[] bytes = javatest.returnAByteArray();
        for (int i=0;i<bytes.length;i++){
            Log.i("myDemo-jni",bytes[i]+"");
        }
        String[] arrays = new String[]{"a","b","c"};
        Log.i("myDemo-jni",javatest.stringManipulator("jesson",arrays));

        //----c调用Java
        javatest.cDiaojavaJingTai();
        javatest.cDiaojava();
        javatest.changeValue();
        Log.i("myDemo-jni",javatest.number+"");
        Log.i("myDemo-jni",javatest.content+"");
    }
}
