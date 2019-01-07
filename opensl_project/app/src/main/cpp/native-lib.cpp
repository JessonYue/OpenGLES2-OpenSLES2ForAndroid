#include <jni.h>
#include <string>

extern "C" JNIEXPORT jstring JNICALL
Java_learning_jesson_com_opensl_1project_MainActivity_stringFromJNI(
        JNIEnv *env,
        jobject /* this */) {
    std::string hello = "Hello from C++";
    return env->NewStringUTF(hello.c_str());
}
