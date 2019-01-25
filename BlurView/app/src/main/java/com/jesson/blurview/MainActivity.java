package com.jesson.blurview;

import androidx.appcompat.app.AppCompatActivity;

import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.os.Bundle;
import android.widget.ImageView;
import android.widget.TextView;

import com.jesson.blurviewlib.BlurUtil;

public class MainActivity extends AppCompatActivity {


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);



        ImageView imageView = findViewById(R.id.image);
        Bitmap bm = BitmapFactory.decodeResource(getResources(), R.mipmap.bg);
        imageView.setImageBitmap(BlurUtil.blur(bm));
    }

}
