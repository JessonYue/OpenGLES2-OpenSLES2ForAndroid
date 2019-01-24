package openssl.jesson.com.openssl;

import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Base64;
import android.util.Log;
import android.widget.TextView;

public class MainActivity extends AppCompatActivity {

    private static final String TEST_DATA = "13bcdfasg5456!@#$%^&*()_+=~`/|?><:'-+*./数据加解密测试—13bcdfasg5456!@#$%^&*()_+=~`/|?><:'-+*./数据加解密测试—13bcdfasg5456!@#$%^&*()_+=~`/|?><:'-+*./数据加解密测试—13bcdfasg5456!@#$%^&*()_+=~`/|?><:'-+*./数据加解密测试—13bcdfasg5456!@#$%^&*()_+=~`/|?><:'-+*./数据加解密测试—13bcdfasg5456!@#$%^&*()_+=~`/|?><:'-+*./数据加解密测试—13bcdfasg5456!@#$%^&*()_+=~`/|?><:'-+*./数据加解密测试—13bcdfasg5456!@#$%^&*()_+=~`/|?><:'-+*./数据加解密测试—13bcdfasg5456!@#$%^&*()_+=~`/|?><:'-+*./数据加解密测试—13bcdfasg5456!@#$%^&*()_+=~`/|?><:'-+*./数据加解密测试—13bcdfasg5456!@#$%^&*()_+=~`/|?><:'-+*./数据加解密测试—13bcdfasg5456!@#$%^&*()_+=~`/|?><:'-+*./数据加解密测试—13bcdfasg5456!@#$%^&*()_+=~`/|?><:'-+*./数据加解密测试—13bcdfasg5456!@#$%^&*()_+=~`/|?><:'-+*./数据加解密测试—13bcdfasg5456!@#$%^&*()_+=~`/|?><:'-+*./数据加解密测试—13bcdfasg5456!@#$%^&*()_+=~`/|?><:'-+*./数据加解密测试—13bcdfasg5456!@#$%^&*()_+=~`/|?><:'-+*./数据加解密测试—13bcdfasg5456!@#$%^&*()_+=~`/|?><:'-+*./数据加解密测试—13bcdfasg5456!@#$%^&*()_+=~`/|?><:'-+*./数据加解密测试—13bcdfasg5456!@#$%^&*()_+=~`/|?><:'-+*./数据加解密测试—13bcdfasg5456!@#$%^&*()_+=~`/|?><:'-+*./数据加解密测试—13bcdfasg5456!@#$%^&*()_+=~`/|?><:'-+*./数据加解密测试—13bcdfasg5456!@#$%^&*()_+=~`/|?><:'-+*./数据加解密测试—13bcdfasg5456!@#$%^&*()_+=~`/|?><:'-+*./数据加解密测试—13bcdfasg5456!@#$%^&*()_+=~`/|?><:'-+*./数据加解密测试—13bcdfasg5456!@#$%^&*()_+=~`/|?><:'-+*./数据加解密测试—13bcdfasg5456!@#$%^&*()_+=~`/|?><:'-+*./数据加解密测试—13bcdfasg5456!@#$%^&*()_+=~`/|?><:'-+*./数据加解密测试—13bcdfasg5456!@#$%^&*()_+=~`/|?><:'-+*./数据加解密测试—13bcdfasg5456!@#$%^&*()_+=~`/|?><:'-+*./数据加解密测试—13bcdfasg5456!@#$%^&*()_+=~`/|?><:'-+*./数据加解密测试—13bcdfasg5456!@#$%^&*()_+=~`/|?><:'-+*./数据加解密测试—13bcdfasg5456!@#$%^&*()_+=~`/|?><:'-+*./数据加解密测试—13bcdfasg5456!@#$%^&*()_+=~`/|?><:'-+*./数据加解密测试—13bcdfasg5456!@#$%^&*()_+=~`/|?><:'-+*./数据加解密测试—13bcdfasg5456!@#$%^&*()_+=~`/|?><:'-+*./数据加解密测试—13bcdfasg5456!@#$%^&*()_+=~`/|?><:'-+*./数据加解密测试—13bcdfasg5456!@#$%^&*()_+=~`/|?><:'-+*./数据加解密测试—13bcdfasg5456!@#$%^&*()_+=~`/|?><:'-+*./数据加解密测试—13bcdfasg5456!@#$%^&*()_+=~`/|?><:'-+*./数据加解密测试—13bcdfasg5456!@#$%^&*()_+=~`/|?><:'-+*./数据加解密测试—13bcdfasg5456!@#$%^&*()_+=~`/|?><:'-+*./数据加解密测试—13bcdfasg5456!@#$%^&*()_+=~`/|?><:'-+*./数据加解密测试—13bcdfasg5456!@#$%^&*()_+=~`/|?><:'-+*./数据加解密测试—13bcdfasg5456!@#$%^&*()_+=~`/|?><:'-+*./数据加解密测试—13bcdfasg5456!@#$%^&*()_+=~`/|?><:'-+*./数据加解密测试—13bcdfasg5456!@#$%^&*()_+=~`/|?><:'-+*./数据加解密测试—13bcdfasg5456!@#$%^&*()_+=~`/|?><:'-+*./数据加解密测试—13bcdfasg5456!@#$%^&*()_+=~`/|?><:'-+*./数据加解密测试—13bcdfasg5456!@#$%^&*()_+=~`/|?><:'-+*./数据加解密测试—13bcdfasg5456!@#$%^&*()_+=~`/|?><:'-+*./数据加解密测试—13bcdfasg5456!@#$%^&*()_+=~`/|?><:'-+*./数据加解密测试—13bcdfasg5456!@#$%^&*()_+=~`/|?><:'-+*./数据加解密测试—13bcdfasg5456!@#$%^&*()_+=~`/|?><:'-+*./数据加解密测试—13bcdfasg5456!@#$%^&*()_+=~`/|?><:'-+*./数据加解密测试—13bcdfasg5456!@#$%^&*()_+=~`/|?><:'-+*./数据加解密测试—13bcdfasg5456!@#$%^&*()_+=~`/|?><:'-+*./数据加解密测试—13bcdfasg5456!@#$%^&*()_+=~`/|?><:'-+*./数据加解密测试—13bcdfasg5456!@#$%^&*()_+=~`/|?><:'-+*./数据加解密测试—13bcdfasg5456!@#$%^&*()_+=~`/|?><:'-+*./数据加解密测试—13bcdfasg5456!@#$%^&*()_+=~`/|?><:'-+*./数据加解密测试—13bcdfasg5456!@#$%^&*()_+=~`/|?><:'-+*./数据加解密测试—13bcdfasg5456!@#$%^&*()_+=~`/|?><:'-+*./数据加解密测试—13bcdfasg5456!@#$%^&*()_+=~`/|?><:'-+*./数据加解密测试—13bcdfasg5456!@#$%^&*()_+=~`/|?><:'-+*./数据加解密测试—13bcdfasg5456!@#$%^&*()_+=~`/|?><:'-+*./数据加解密测试—13bcdfasg5456!@#$%^&*()_+=~`/|?><:'-+*./数据加解密测试—";
    private static final String TEST_KEY = "JA2F8AKJF3D7HF12";

    private static final String TEST_PRIVATE_KEY = "-----BEGIN PRIVATE KEY-----\n" + "MIICdQIBADANBgkqhkiG9w0BAQEFAASCAl8wggJbAgEAAoGBAKLy3gcwWwy+mhcr\n" + "gcXccrRu+UQUHDwvogZbGjbBDGsyt5hY69FtwIy/45tdj42xb4Tr0o1qKjuXHmIt\n" + "zWAlgm+e9Fi4vwj6sIIbgdvYhi2dm/N2abNzEMJ2WsG2kei64qsaZtlawWv9k2GG\n" + "ChP63MR79Z9+ucBzu+SZp+XrEt9DAgMBAAECgYB4Tr51KlOfJj7YqounDWs3ItQx\n" + "WnO6UCTdcnf5QzErGIgLGGQL/W9zu92NgVeS8xV2WzLarC7AToPlUxHWUftpxqCa\n" + "alQ+HtJ2zROnbblMwmEcnwsPXD8SncjJGDg1mSxkhi/jw1riPg36Exw1VGgmww4b\n" + "+iMboCv3ApBDdxn7yQJBAM7rgodIHGf11d9+TO+PUkglc9AfCDMXQraDirU6JjCh\n" + "6AVJXH76k2oLz4DCvd3CCBcM5qGmdAzTK/X1MSToGgUCQQDJmVtKiJkPOe/N2Vi3\n" + "MkIIalnqZ9GFYtDjUV3dgI1QVgLQ8qpN2y98j8PU9nM/BpU0fU4qSX36vPCfYn0e\n" + "mS6nAkABiAKmR6VWK56Skde16iScvhI2VxRzdFedDCopny2LLJeP+nQByI7wuPen\n" + "J0nKa1Yt/X1zcsznD2UC4/aiJEmVAkACL+a8pUS71I4UdqIuwp3Sx4yYLW4pe0v2\n" + "22AgUg+2amh3adqNI66dNFYUjmPrsB+YRS++57M1MC2QHRpsZY8LAkAKFNUtX47a\n" + "4LYofojZrEdcz9O8xisB4bsv04G+WiM4bqTrlQo/6Y3YofvaP5jGSwBW8K/w6KPX\n" + "D0VGzyfqFiL7\n" + "-----END PRIVATE KEY-----\n";

    private static final String TEST_PUBLIC_KEY = "-----BEGIN PUBLIC KEY-----\n" + "MIGfMA0GCSqGSIb3DQEBAQUAA4GNADCBiQKBgQCi8t4HMFsMvpoXK4HF3HK0bvlE\n" + "FBw8L6IGWxo2wQxrMreYWOvRbcCMv+ObXY+NsW+E69KNaio7lx5iLc1gJYJvnvRY\n" + "uL8I+rCCG4Hb2IYtnZvzdmmzcxDCdlrBtpHouuKrGmbZWsFr/ZNhhgoT+tzEe/Wf\n" + "frnAc7vkmafl6xLfQwIDAQAB\n" + "-----END PUBLIC KEY-----\n";
    private static final String TAG = "openssl";


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        JNIUTILS jniutils = new JNIUTILS();
        Log.i(TAG, "原始数据长度：->" + TEST_DATA.length());
        byte[] encodeByHmacSHA1 = jniutils.encodeByHmacSHA1(this, TEST_DATA.getBytes());
        String hmacSHA1 = Base64.encodeToString(encodeByHmacSHA1, Base64.NO_WRAP);
        Log.i(TAG, "hmacSHA1签名编码->" + hmacSHA1 + "\n签名长度->" + hmacSHA1.length());

    }


}
