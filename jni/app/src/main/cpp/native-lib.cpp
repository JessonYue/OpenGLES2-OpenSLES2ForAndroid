#include <jni.h>
#include <string>
#include <vector>
#include <algorithm>
#include <iostream>
#include <cmath>
#include<android/log.h>

#define TAG "myDemo-jni" // 这个是自定义的LOG的标识
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG,TAG ,__VA_ARGS__) // 定义LOGD类型
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO,TAG ,__VA_ARGS__) // 定义LOGI类型
#define LOGW(...) __android_log_print(ANDROID_LOG_WARN,TAG ,__VA_ARGS__) // 定义LOGW类型
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR,TAG ,__VA_ARGS__) // 定义LOGE类型
#define LOGF(...) __android_log_print(ANDROID_LOG_FATAL,TAG ,__VA_ARGS__) // 定义LOGF类型


using namespace std;

extern "C"
JNIEXPORT void JNICALL
Java_jni_jesson_com_jni_JavaTest_printMethod(JNIEnv *env, jobject instance) {
    LOGD("Native method called. Printing jesson." );
}


extern "C"
JNIEXPORT jbyteArray JNICALL
Java_jni_jesson_com_jni_JavaTest_returnAByteArray(JNIEnv *env, jobject instance) {
    jbyteArray __ba = env->NewByteArray(3);
    std::vector<unsigned char> __c_vec(3);
    __c_vec[0] = 0;
    __c_vec[1] = 1;
    __c_vec[2] = 1;
    unsigned char *__c_ptr = __c_vec.data();
    env->SetByteArrayRegion(__ba, 0, 3, reinterpret_cast<jbyte *>(__c_ptr));
    std::cout << "Printing Byte Array members..." << std::endl;
    std::for_each(__c_vec.begin(), __c_vec.end(), [](const char &c) { std::cout << c; });
    std::cout << std::endl << std::endl;
    return __ba;
}


extern "C"
JNIEXPORT jstring JNICALL
Java_jni_jesson_com_jni_JavaTest_stringManipulator(JNIEnv *env, jobject instance, jstring s_,
                                                   jobjectArray s1) {
    std::string s = env->GetStringUTFChars(s_, NULL);
    std::cout << "s  is : " << s << std::endl;
    // iterate over
    for(int i = 0; i < env->GetArrayLength(s1); i++) {
        std::cout
                << env->GetStringUTFChars((jstring)env->GetObjectArrayElement(s1, (jsize)i), JNI_FALSE)
                << std::endl;
    }
    s.append("::::::THIS IS APPENDED TEXT!!!! WARNING!!! WARNING!!!! :)");
    return env->NewStringUTF(s.data());
}

extern "C"
JNIEXPORT void JNICALL
Java_jni_jesson_com_jni_JavaTest_cDiaojavaJingTai(JNIEnv *env, jobject instance) {

    //获取类Class对象
    jclass clazz = env->FindClass("jni/jesson/com/jni/JavaTest");

    //获取java函数的id值
    jmethodID id = env->GetStaticMethodID(clazz,"cDiao","(Ljava/lang/String;)V");

    if(id==0){
        LOGD("find method1 error");
        return;
    }
    //定义参数
    jstring msg = env->NewStringUTF("jniCodeDiao.c calljavaMethod");

    //进行方法的调用
    env->CallStaticVoidMethod(clazz,id,msg);
}

extern "C"
JNIEXPORT void JNICALL
Java_jni_jesson_com_jni_JavaTest_cDiaojava(JNIEnv *env, jobject instance) {
    //获取类Class对象
    //jclass clazz = env->FindClass("jni/jesson/com/jni/JavaTest");

    //获得obj对象
    jclass cla = env->GetObjectClass(instance);

    //获取java函数的id值
    jmethodID id = env->GetMethodID(cla,"cNewObjDJ","()V");

    //进行方法的调用
    env->CallVoidMethod(instance,id);
}

extern "C"
JNIEXPORT void JNICALL
Java_jni_jesson_com_jni_JavaTest_changeValue(JNIEnv *env, jobject obj) {


    jclass clazz = env->GetObjectClass(obj);
    jfieldID int_fieldID = env->GetFieldID(clazz, "number", "I");
    jint number = env->GetIntField(obj, int_fieldID);
    //change number value
    number = (int)pow(number, 2);
    env->SetIntField(obj, int_fieldID, number);

    //be careful Ljava/lang/String
    jfieldID string_fieldID = env->GetFieldID(clazz, "content", "Ljava/lang/String;");
    jstring content = (jstring)env->GetObjectField(obj, string_fieldID);
    const char* cc = env->GetStringUTFChars(content, 0);
    env->ReleaseStringUTFChars(content, cc);


    char hello[] = "I'm from C++!\n";
    jstring jhello = env->NewStringUTF(hello);
    //change content value
    env->SetObjectField(obj, string_fieldID, jhello);
    return;

}

/**
 * 动态调用  不需要包名和类的路径来确定位置
 */
extern "C"
//JNIEnv *env, jobject instance 如果传参数这两个参数必须存在
jint dynamicTest(JNIEnv *env, jobject instance, jint i) {
    LOGE("我是被动态注册的方法dynamicTest");
    return i;
}


extern "C"
void doit(JNIEnv *env, jobject obj){
    LOGE("我是被动态注册的方法doit");
    jthrowable exc = NULL;
    jclass clazz = env->GetObjectClass(obj);
    jmethodID mid = env->GetStaticMethodID(clazz,"exceptionCallback","()V");
    if (mid != NULL) {
        env->CallStaticVoidMethod(clazz,mid);
    }
    printf("In C: Java_com_study_jnilearn_JNIException_doit-->called!!!!");
    if (env->ExceptionCheck()) {  // 检查JNI调用是否有引发异常
        env->ExceptionDescribe();
        env->ExceptionClear();        // 清除引发的异常，在Java层不会打印异常的堆栈信息
        env->ThrowNew(env->FindClass("java/lang/Exception"),"JNI抛出的异常！");
        return;
    }
    mid = env->GetStaticMethodID(clazz,"normalCallback","()V");
    if (mid != NULL) {
        env->CallStaticVoidMethod(clazz,mid);
    }

}


extern "C"
void mainThread(JNIEnv *env, jobject obj){
env->
}


extern "C"
void setJNIEnv(JNIEnv *env, jobject obj){

}



JavaVM *_vm;
//类名
static const char *mClassName = "jni/jesson/com/jni/JavaTest";
//静态的Jni native 方法数组
static const JNINativeMethod method[] = {
        //方法名    签名    本地方法
        {"dynamicJavaTest", "(I)I", (int *) dynamicTest},
        {"doit", "()V", (void *)doit},
        {"mainThread", "()V", (void *)mainThread},
        {"setJNIEnv", "()V", (void *)setJNIEnv}
};

//返回Jni 版本
int JNI_OnLoad(JavaVM *vm, void *r) {
    LOGE("JNI_OnLoad");
    _vm = vm;
    JNIEnv *env = 0;
    //获得JNIEnv 这里会返回一个值 小于0 代表失败
    jint res = vm->GetEnv((void **) (&env), JNI_VERSION_1_6);
    //判断返回结果
    if (res != JNI_OK) {
        return -1;
    }
    //根据类名找到类，注意有native的类不能被混淆
    jclass jcls = env->FindClass(mClassName);
    //动态注册  第一个参数 类  第二个参数 方法数组  第三个参数 注册多少个方法
    env->RegisterNatives(jcls, method, sizeof(method) / sizeof(JNINativeMethod));
    return JNI_VERSION_1_6;
}