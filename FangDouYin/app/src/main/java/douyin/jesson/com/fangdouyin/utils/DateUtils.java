package douyin.jesson.com.fangdouyin.utils;

import java.text.SimpleDateFormat;
import java.util.Date;



public class DateUtils {

    public static String covertToDate(long duration){
        Date date = new Date(duration);
        SimpleDateFormat format = new SimpleDateFormat("mm:ss");
        return format.format(date);
    }
}
