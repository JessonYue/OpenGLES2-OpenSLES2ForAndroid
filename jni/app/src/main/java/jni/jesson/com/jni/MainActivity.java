package jni.jesson.com.jni;

import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.view.View;


public class MainActivity extends AppCompatActivity implements View.OnClickListener {

    JavaTest javatest;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        findViewById(R.id.yichang).setOnClickListener(this);
        findViewById(R.id.thread).setOnClickListener(this);


        javatest = new JavaTest();


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
        javatest.dynamicJavaTest(100);

        javatest.setJNIEnv();
    }

    @Override
    public void onClick(View v) {
        switch (v.getId()){
            case R.id.yichang:
                try {
                    javatest.doit();
                } catch (Exception e) {
                    e.printStackTrace();
                }
                break;
            case R.id.thread:
                javatest.mainThread();
                break;
        }
    }
}
