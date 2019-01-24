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


JavaVM * jvm;
jmethodID callbackId;
jobject  obj;
static int startNum;
static int endNum;
static const char *classPathPrime = "jni/jesson/com/jni/thread/PrimeNative";


static JNINativeMethod methodPrime[] = {
        {"init", "(II)V", (void *)jni_jesson_com_jni_thread_PrimeNative_init}
};

//static JNINativeMethod methodExec[] = {
//        {"exec","()V", (void*) Java_com_zuoshaohua_threadtest_ActNative_nativeExec}
//};


static int registerNativeMethods(JNIEnv* env, const char* className, JNINativeMethod* gMethods, int numMethods){

    jclass clz = env->FindClass(className);                    //通过完整的类路径得jclass结构体.
    env->RegisterNatives(clz, gMethods, numMethods);
    return JNI_TRUE;
}

static int registerNatives(JNIEnv* env){
    registerNativeMethods(env, classPathPrime, methodPrime, sizeof(methodPrime) / sizeof(methodPrime[0]));
    //registerNativeMethods(env, classPathAct, methodExec, sizeof(methodExec) / sizeof(methodExec[0]));
    return JNI_TRUE;

}

jint JNI_OnLoad(JavaVM* vm, void* reserved){
    JNIEnv *env;
    jvm = vm;
    if ((jvm->GetEnv((void**) &env, JNI_VERSION_1_4) != JNI_OK)) //将JNI的版本设置为1.4,并通过jvm获取JNIEnv结构体
        return -1;
    if (registerNatives(env) != JNI_TRUE)                             //注册我们的本地方法到VM中.
        return -1;
    return JNI_VERSION_1_4;
}






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



int isPrime(int num)
{
    int i = 0;
    for(i = 2; i*i <= num; i++)     //这里必须是 i*i<= num, 比如25,如果 i*i<25,
    {                               //那么这个数就会被判为奇数.以为,i=5的时候. i*i = <span style="font-family:Courier New;">25,退出循环,</span>
        if( num%i == 0 ){
            return 0;
        }
    }
    return 1;
}


extern "C"
JNIEXPORT void JNICALL
Java_jni_jesson_com_jni_thread_PrimeNative_init(JNIEnv *env, jobject instance, jint start,
                                                jint end) {

//获取start,和end.保存到一个起来．
    __android_log_print(ANDROID_LOG_INFO, "PrimeNative", "start = %d, end =%d\n", start, end);
    startNum = start;
    endNum = end;
    //1. 获取jclass对象.
    jclass clz =env->GetObjectClass(instance);
    //2. 获取callBack方法.
    callbackId = env->GetMethodID(clz,"callBack","(I)V");
    //3. 将clz转化成全局变量.
    obj = env->NewGlobalRef(instance);

    int i = 0 ;
    for(i  =startNum; i<endNum; i++)
    {
        if(isPrime(i))
        {
            __android_log_print(ANDROID_LOG_INFO, "ActNative", "prime = %d", i);
            env->CallVoidMethod(obj,callbackId,i);    //回调java层方法,将数据显示到Java页面.
        }
    }
}