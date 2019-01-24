package openssl.jesson.com.openssl;

import android.content.Context;

public class JNIUTILS {

    static {
        System.loadLibrary("native-lib");
    }

    /**
     * HmacSHA1签名
     *
     * @param context
     * @param src
     * @return
     */
    public native byte[] encodeByHmacSHA1(Context context, byte[] src);

}
