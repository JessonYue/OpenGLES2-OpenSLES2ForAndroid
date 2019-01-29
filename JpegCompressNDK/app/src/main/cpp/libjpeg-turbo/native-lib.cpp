#include <jni.h>
#include <string>

#include <android/log.h>
#include <android/bitmap.h>
#include <csetjmp>


// log宏定义
#define TAG "Native_JpegCompress_Jni"
#define LOG_D(...) __android_log_print(ANDROID_LOG_DEBUG,TAG ,__VA_ARGS__)

extern "C"{
#include "include/jpeglib.h"
#include "include/cdjpeg.h"          /* Common decls for cjpeg/djpeg applications */
#include "include/jversion.h"        /* for version message */
#include "include/config.h"
}

typedef uint8_t BYTE;

int generateJPEG(BYTE *tmpdata, jint w, jint h, jint quality, const char *name, jboolean optimize);

extern "C" JNIEXPORT jstring JNICALL
Java_com_jesson_blurview_MainActivity_stringFromJNI(
        JNIEnv *env,
        jobject /* this */) {
    std::string hello = "Hello from C++";
    return env->NewStringUTF(hello.c_str());
}


//实现一些jni的工具方法
char* jstringTostring(JNIEnv *env,jbyteArray barry){
    char* str = NULL;
    jsize length = env->GetArrayLength(barry);
    jbyte* ba = env->GetByteArrayElements(barry, 0);
    if (length > 0) {
        str = (char*) malloc(length + 1);
        memcpy(str, ba, length);
        str[length] = 0;
    }
    env->ReleaseByteArrayElements(barry, ba, 0);
    return str;
}

jbyteArray stoJstring(JNIEnv* env, const char* pat,int len) {
    jbyteArray bytes = env->NewByteArray(len);
    env->SetByteArrayRegion(bytes, 0, len, reinterpret_cast<const jbyte *>(pat));
    jsize alen = env->GetArrayLength(bytes);
    return bytes;
}

void jstringTostring(JNIEnv* env, jstring jstr, char * output, int * de_len) {
    *output = NULL;
    jclass clsstring = env->FindClass("java/lang/String");
    jstring strencode = env->NewStringUTF("utf-8");
    jmethodID mid = env->GetMethodID(clsstring, "getBytes",
                                        "(Ljava/lang/String;)[B");
    jbyteArray barr = (jbyteArray)env->CallObjectMethod(jstr, mid,
                                                           strencode);
    jsize alen = env->GetArrayLength(barr);
    *de_len = alen;
    jbyte* ba = env->GetByteArrayElements(barr, JNI_FALSE);
    if (alen > 0) {
        output = (char*) malloc(alen + 1);
        memcpy(output, ba, alen);
        output[alen] = 0;
    }
    env->ReleaseByteArrayElements(barr, ba, 0);
}

//---------------------------业务代码------------------------------------

extern "C"
JNIEXPORT jstring JNICALL
Java_com_jesson_jpegcompressndk_MainActivity_compressBitmap(
        JNIEnv *env, jclass type, jobject bit, jint w, jint h, jint quality,
        jbyteArray fileNameBytes_, jboolean optimize) {

    char * fileName = jstringTostring(env, fileNameBytes_);


    AndroidBitmapInfo infocolor;
    BYTE * addrPtr;
    int ret;
    if ((ret = AndroidBitmap_getInfo(env, bit, &infocolor)) < 0) {
        LOG_D("AndroidBitmap_getInfo() failed ! error=%d", ret);
        return env->NewStringUTF("0");;
    }

    if ((ret = AndroidBitmap_lockPixels(env, bit, reinterpret_cast<void **>(&addrPtr))) < 0) {
        LOG_D("AndroidBitmap_lockPixels() failed ! error=%d", ret);
    }

    BYTE * data;
    BYTE *tmpdata;
    BYTE r, g, b;
    data = NULL;
    data = static_cast<BYTE *>(malloc(w * h * 3));
    tmpdata = data;
    int j = 0, i = 0;
    int color;
    for (i = 0; i < h; i++) {
        for (j = 0; j < w; j++) {
            color = *((int *) addrPtr);
            r = ((color & 0x00FF0000) >> 16);
            g = ((color & 0x0000FF00) >> 8);
            b = color & 0x000000FF;
            *data = b;
            *(data + 1) = g;
            *(data + 2) = r;
            data = data + 3;
            addrPtr += 4;
        }
    }
    AndroidBitmap_unlockPixels(env, bit);
    int resultCode= generateJPEG(tmpdata, w, h, quality, fileName, optimize);
    free(tmpdata);
    if(resultCode==0){
        jstring result=env->NewStringUTF("error");
        return result;
    }

    return env->NewStringUTF("1"); //1:sucess
}

struct my_error_mgr {
    struct jpeg_error_mgr pub;
    jmp_buf setjmp_buffer;
};
char *error;
typedef struct my_error_mgr * my_error_ptr;

METHODDEF(void)
my_error_exit (j_common_ptr cinfo)
{
    my_error_ptr myerr = (my_error_ptr) cinfo->err;
    (*cinfo->err->output_message) (cinfo);
    error= const_cast<char *>(myerr->pub.jpeg_message_table[myerr->pub.msg_code]);
    LOG_D("jpeg_message_table[%d]:%s", myerr->pub.msg_code,myerr->pub.jpeg_message_table[myerr->pub.msg_code]);
    // LOGE("addon_message_table:%s", myerr->pub.addon_message_table);
//  LOGE("SIZEOF:%d",myerr->pub.msg_parm.i[0]);
//  LOGE("sizeof:%d",myerr->pub.msg_parm.i[1]);
    longjmp(myerr->setjmp_buffer, 1);
}

int generateJPEG(BYTE *data, jint w, jint h, jint quality, const char *name, jboolean optimize) {

    int nComponent = 3;

    struct jpeg_compress_struct jcs;

    struct my_error_mgr jem;

    jcs.err = jpeg_std_error(&jem.pub);
    jem.pub.error_exit = my_error_exit;
    if (setjmp(jem.setjmp_buffer)) {
        return 0;
    }
    jpeg_create_compress(&jcs);
    FILE* f = fopen(name, "wb");
    if (f == NULL) {
        return 0;
    }
    jpeg_stdio_dest(&jcs, f);
    jcs.image_width = w;
    jcs.image_height = h;
    if (optimize) {
        LOG_D("optimize==ture");
    } else {
        LOG_D("optimize==false");
    }

    jcs.arith_code = false;
    jcs.input_components = nComponent;
    if (nComponent == 1)
        jcs.in_color_space = JCS_GRAYSCALE;
    else
        jcs.in_color_space = JCS_RGB;

    jpeg_set_defaults(&jcs);
    jcs.optimize_coding = optimize;
    jpeg_set_quality(&jcs, quality, true);

    jpeg_start_compress(&jcs, TRUE);

    JSAMPROW row_pointer[1];
    int row_stride;
    row_stride = jcs.image_width * nComponent;
    while (jcs.next_scanline < jcs.image_height) {
        row_pointer[0] = &data[jcs.next_scanline * row_stride];

        jpeg_write_scanlines(&jcs, row_pointer, 1);
    }

    if (jcs.optimize_coding) {
        LOG_D("optimize==ture");
    } else {
        LOG_D("optimize==false");
    }
    jpeg_finish_compress(&jcs);
    jpeg_destroy_compress(&jcs);
    fclose(f);

    return 1;
}