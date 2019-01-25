#include <jni.h>
#include <string>
#include <android/bitmap.h>
#include "base64.h"

#include <android/log.h>


#define eprintf(...) __android_log_print(ANDROID_LOG_ERROR,"@",__VA_ARGS__)


using namespace std;
jobject generateBitmap(JNIEnv *env, uint32_t width, uint32_t height);


extern "C" JNIEXPORT jstring JNICALL
Java_com_jesson_photobase64_MainActivity_stringFromJNI(
        JNIEnv *env,
        jobject /* this */) {
    std::string hello = "Hello from C++";
    return env->NewStringUTF(hello.c_str());
}

uint32_t bitmapwidth;
uint32_t bitmapheight;

/**
 * 测试bitmap
 */
extern "C"
JNIEXPORT jobject JNICALL
Java_com_jesson_photobase64_MainActivity_greenBitmap(JNIEnv *env, jclass type, jobject src) {

    unsigned char *srcData = NULL;
    AndroidBitmap_lockPixels(env, src, (void**)&srcData);
    AndroidBitmapInfo srcInfo;
    AndroidBitmap_getInfo(env, src, &srcInfo);

    int pixel = srcInfo.width * srcInfo.height;
    bitmapheight = srcInfo.height;
    bitmapwidth = srcInfo.width;
    for(int i=0;i<pixel;i++)
    {
        srcData [4*i] = (unsigned char)0;//red
        srcData [4*i+1] = (unsigned char)255;//green
        srcData [4*i+2] = (unsigned char)0;//blue
        srcData [4*i+3] = (unsigned char)255;//alpha
    }

    AndroidBitmap_unlockPixels(env, src);
    return src;


}

extern "C"
JNIEXPORT jstring JNICALL
Java_com_jesson_photobase64_MainActivity_encodeBitmapByBase64(JNIEnv *env, jclass type,
                                                              jobject bitmap) {
    unsigned char *srcData = NULL;
    AndroidBitmap_lockPixels(env, bitmap, (void**)&srcData);
    AndroidBitmapInfo srcInfo;
    AndroidBitmap_getInfo(env, bitmap, &srcInfo);
    eprintf("bitmap width: %d, height: %d, format: %d, stride: %d", srcInfo.width, srcInfo.height,
         srcInfo.format, srcInfo.stride);
    int length = srcInfo.stride * srcInfo.height;
    for (int i = 0; i < length; ++i) {
        eprintf("value: %x", srcData[i]);
    }


    AndroidBitmap_unlockPixels(env, bitmap);
    //base64
    string str = base64_encode(srcData, sizeof(srcData));
    return env->NewStringUTF(str.c_str());
}

extern "C"
JNIEXPORT jobject JNICALL
Java_com_jesson_photobase64_MainActivity_decodeBitmapByBase64(JNIEnv *env, jclass type,
                                                              jstring bitmapbase64str_) {
    const char *bitmapbase64str = env->GetStringUTFChars(bitmapbase64str_, 0);
    //decode base64
    unsigned char *srcData =  nullptr;
    jobject newbitmap = generateBitmap(env,bitmapwidth,bitmapheight);
    AndroidBitmap_lockPixels(env, newbitmap, (void**)&srcData);
    AndroidBitmapInfo srcInfo;
    AndroidBitmap_getInfo(env, newbitmap, &srcInfo);
    //todo 这里的base64 解码不对
    srcData = (unsigned char *) base64_decode(bitmapbase64str).c_str();
    AndroidBitmap_unlockPixels(env, newbitmap);
    env->ReleaseStringUTFChars(bitmapbase64str_, bitmapbase64str);

    string str = base64_decode("AP8A/wD/AP8=");
    unsigned char *srcData2 = (unsigned char *) base64_decode(bitmapbase64str).c_str();

    return newbitmap;
}


jobject generateBitmap(JNIEnv *env, uint32_t width, uint32_t height) {

    jclass bitmapCls = env->FindClass("android/graphics/Bitmap");
    jmethodID createBitmapFunction = env->GetStaticMethodID(bitmapCls,
                                                            "createBitmap",
                                                            "(IILandroid/graphics/Bitmap$Config;)Landroid/graphics/Bitmap;");
    jstring configName = env->NewStringUTF("ARGB_8888");
    jclass bitmapConfigClass = env->FindClass("android/graphics/Bitmap$Config");
    jmethodID valueOfBitmapConfigFunction = env->GetStaticMethodID(
            bitmapConfigClass, "valueOf",
            "(Ljava/lang/String;)Landroid/graphics/Bitmap$Config;");

    jobject bitmapConfig = env->CallStaticObjectMethod(bitmapConfigClass,
                                                       valueOfBitmapConfigFunction, configName);

    jobject newBitmap = env->CallStaticObjectMethod(bitmapCls,
                                                    createBitmapFunction,
                                                    width,
                                                    height, bitmapConfig);
    return newBitmap;
}