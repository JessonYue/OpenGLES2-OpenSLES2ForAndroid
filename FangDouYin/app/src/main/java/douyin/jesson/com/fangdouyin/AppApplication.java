package douyin.jesson.com.fangdouyin;

import android.app.Application;
import android.content.Context;
import android.util.Log;

public class AppApplication extends Application {
    private static Context mContext;



    @Override
    public void onCreate() {
        super.onCreate();
        mContext = this;
        Log.e("thread","线程是"+Thread.currentThread());
        Constants.init(this);
    }

    @Override
    protected void attachBaseContext(Context base) {
        super.attachBaseContext(base);

    }

    public static Context getContext() {
        return mContext;
    }
}
