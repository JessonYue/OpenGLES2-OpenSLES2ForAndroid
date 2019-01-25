package com.jesson.photobase64;

import androidx.appcompat.app.AppCompatActivity;

import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.os.Bundle;
import android.util.Base64;
import android.util.Log;
import android.view.View;
import android.widget.ImageView;
import android.widget.TextView;

public class MainActivity extends AppCompatActivity {

    static {
        System.loadLibrary("native-lib");
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        ImageView imageView = findViewById(R.id.image);
        Bitmap bm = BitmapFactory.decodeResource(getResources(), R.mipmap.bg);
        Bitmap update = greenBitmap(bm);
        //imageView.setImageBitmap(update);


        //测试加密
        Log.e("tag",encodeBitmapByBase64(bm));
        //base64=>bitmap
        imageView.setImageBitmap(decodeBitmapByBase64(encodeBitmapByBase64(bm)));
    }


    public static native Bitmap greenBitmap(Bitmap bitmap);
    public static native String encodeBitmapByBase64(Bitmap bitmap);
    public static native Bitmap decodeBitmapByBase64(String bitmapbase64str);
}
