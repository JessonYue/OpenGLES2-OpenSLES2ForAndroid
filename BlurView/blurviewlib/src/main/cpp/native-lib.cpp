#include <jni.h>
#include <string>

#include <android/log.h>
#include <android/bitmap.h>
#include "blur.h"

// log宏定义
#define TAG "Native_Blur_Jni"
#define LOG_D(...) __android_log_print(ANDROID_LOG_DEBUG,TAG ,__VA_ARGS__)


extern "C" JNIEXPORT jstring JNICALL
Java_com_jesson_blurview_MainActivity_stringFromJNI(
        JNIEnv *env,
        jobject /* this */) {
    std::string hello = "Hello from C++";
    return env->NewStringUTF(hello.c_str());
}


/**实现blurview**/
extern "C"
JNIEXPORT void JNICALL
Java_com_jesson_blurviewlib_BlurUtil_blurBitmap(JNIEnv *env, jclass type, jobject bitmap, jint r) {

    AndroidBitmapInfo androidBitmapInfo;
    void *pixels;
    if(AndroidBitmap_getInfo(env,bitmap,&androidBitmapInfo)!=ANDROID_BITMAP_RESULT_SUCCESS){
        LOG_D("AndroidBitmap_getInfo error");
        return;
    }

    if(androidBitmapInfo.format!=ANDROID_BITMAP_FORMAT_RGBA_8888 &&
            androidBitmapInfo.format!=ANDROID_BITMAP_FORMAT_RGB_565){
        LOG_D("Only support ANDROID_BITMAP_FORMAT_RGBA_8888 and ANDROID_BITMAP_FORMAT_RGB_565");
        return;
    }

    if (AndroidBitmap_lockPixels(env, bitmap, &pixels) != ANDROID_BITMAP_RESULT_SUCCESS) {
        LOG_D("AndroidBitmap_lockPixels failed!");
        return;
    }
    // 得到宽高
    int h = androidBitmapInfo.height;
    int w = androidBitmapInfo.width;
    if (androidBitmapInfo.format == ANDROID_BITMAP_FORMAT_RGBA_8888) {
        // 调用stackblur.c中的blur_ARGB_8888()或blur_RGB_565()
        pixels = blur_ARGB_8888((int *) pixels, w, h, r);
    } else if (androidBitmapInfo.format == ANDROID_BITMAP_FORMAT_RGB_565) {
        pixels = blur_RGB_565((short *) pixels, w, h, r);
    }
    // 对应上面的AndroidBitmap_lockPixels（）
    AndroidBitmap_unlockPixels(env, bitmap);

}