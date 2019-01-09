package douyin.jesson.com.fangdouyin;

import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.Toast;

import douyin.jesson.com.fangdouyin.activity.BaseActivity;
import douyin.jesson.com.fangdouyin.activity.RecordedActivity;

public class MainActivity extends BaseActivity implements View.OnClickListener {


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        Button recordBtn = findViewById(R.id.record_activity);
        Button moreBtn = findViewById(R.id.more);

        recordBtn.setOnClickListener(this);
        moreBtn.setOnClickListener(this);


    }

    @Override
    public void onClick(View v) {
        switch (v.getId()) {
            case R.id.record_activity:
                startActivity(new Intent(MainActivity.this, RecordedActivity.class));
                break;
            case R.id.more:
                Toast.makeText(this, "等你来实践奇思妙想！！！", Toast.LENGTH_SHORT).show();
                break;
        }
    }
}
