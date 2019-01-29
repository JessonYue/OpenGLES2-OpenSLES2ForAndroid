package com.jesson.jpegcompressndk;

import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Canvas;
import android.graphics.Rect;
import android.os.Environment;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.widget.TextView;

import java.io.IOException;

public class MainActivity extends AppCompatActivity {


    static {
        System.loadLibrary("native-lib");
    }
    private static int DEFAULT_QUALITY = 95;


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        Bitmap bitmap = BitmapFactory.decodeResource(getResources(), R.mipmap.bg);
        try {
            compress(bitmap, Environment.getExternalStorageDirectory().getCanonicalPath()+"/1.jpg");
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    private static String compress(Bitmap bitmap, String fileName) {
        return compressBitmap(bitmap, DEFAULT_QUALITY, fileName);
    }

    public static String compressBitmap(Bitmap bit, int quality, String fileName) {
        Bitmap result = null;
        try {
            result = Bitmap.createBitmap(bit.getWidth(), bit.getHeight(), Bitmap.Config.ARGB_8888);
            Canvas canvas = new Canvas(result);
            Rect rect = new Rect(0, 0, bit.getWidth(), bit.getHeight());// original
            canvas.drawBitmap(bit, null, rect, null);
            return saveBitmap(result, quality, fileName);
        }finally {
            if(result != null){
                result.recycle();
            }
        }
    }

    private static String saveBitmap(Bitmap bit, int quality, String fileName) {
        return compressBitmap(bit, bit.getWidth(), bit.getHeight(), quality, fileName.getBytes(), true);
    }

    //返回值"1"是成功，"0"是失败
    private static native String compressBitmap(Bitmap bit, int w, int h, int quality, byte[] fileNameBytes,
                                                boolean optimize);

    //todo 学员可以在接着写安卓Java层的自己的压缩方法 自己学会性能的对比，并总结输出文档
}
