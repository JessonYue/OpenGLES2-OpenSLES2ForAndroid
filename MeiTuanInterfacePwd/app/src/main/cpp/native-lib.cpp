#include <jni.h>
#include <string>
#include <android/log.h>
#include "md5.h"

// 额外附加的字符串
static char *EXTRA_SIGNATURE = "jesson";
// 校验签名
static int is_verify = 0;
static char *PACKAGE_NAME = "com.meituaninterfacepwd.jesson";
static char *APP_SIGNATURE = "308201dd30820146020101300d06092a864886f70d010105050030373116301406035504030c0d416e64726f69642044656275673110300e060355040a0c07416e64726f6964310b3009060355040613025553301e170d3138303932323033333630375a170d3438303931343033333630375a30373116301406035504030c0d416e64726f69642044656275673110300e060355040a0c07416e64726f6964310b300906035504061302555330819f300d06092a864886f70d010101050003818d0030818902818100c8138e544e868e81f4681898853a16f7be57861e7dfe8817aee8326090fb00f574b27aefd893145860fe6b19071b1af6601af049ab8394babc8a8752b6e711c6cc709ab575ec5c6c116528366e0d8b7c4e85b47e7846e6b86c1a83a08c2dd3369e9cd771a86d2babd5eb6d4e88376bc7c514b5ad516bd686e4b52c9c405d8e970203010001300d06092a864886f70d0101050500038181005037678f9f2f0e7cbcc1f6ccb5be7c3808f0b8e1a167c5db10b9fe446b750d00eae58bfae81b689ab9048b461f4b74b992890a46c57da14a0d7c39581f7c44777cc1bc48c6b9767b85018af81c00f42250bd52566863f55b3d4faaae046ce3cb420bd34738baa9681bc272dc5be58a81f0db3d99806b1da0ab2f77ddbfcb6bab";


extern "C"
JNIEXPORT jstring JNICALL
Java_com_meituaninterfacepwd_jesson_SignatureUtils_signatureParams(JNIEnv *env, jclass type,
                                                                   jstring params_) {
    if (is_verify == 0) {
        return env->NewStringUTF("error_signature");
    }

    const char *params = env->GetStringUTFChars(params_, 0);

    // MD5 签名规则，加点料
    // 1. 字符串前面加点料
    std::string signature_str(params);
    signature_str.insert(0, EXTRA_SIGNATURE);
    // 2. 后面去掉两位
    signature_str = signature_str.substr(0, signature_str.length() - 2);

    // 3. md5 去加密 C++ 和 Java 是一样的，唯一不同就是需要自己回收内存
    string md5_str = MD5(signature_str).toStr();

    // 释放资源
    env->ReleaseStringUTFChars(params_, params);
    return env->NewStringUTF(md5_str.c_str());
}


extern "C"
JNIEXPORT void JNICALL
Java_com_meituaninterfacepwd_jesson_SignatureUtils_signatureVerify(JNIEnv *env, jclass type,
                                                                   jobject context) {
// 1. 获取包名
    jclass j_clz = env->GetObjectClass(context);
    jmethodID j_mid = env->GetMethodID(j_clz, "getPackageName", "()Ljava/lang/String;");
    jstring j_package_name = (jstring) env->CallObjectMethod(context, j_mid);
    // 2 . 比对包名是否一样
    const char *c_package_name = env->GetStringUTFChars(j_package_name, NULL);
    if (strcmp(c_package_name, PACKAGE_NAME) != 0) {
        return;
    }
    // 3. 获取签名
    // 3.1 获取 PackageManager
    j_mid = env->GetMethodID(j_clz, "getPackageManager", "()Landroid/content/pm/PackageManager;");
    jobject pack_manager = env->CallObjectMethod(context, j_mid);
    // 3.2 获取 PackageInfo
    j_clz = env->GetObjectClass(pack_manager);
    j_mid = env->GetMethodID(j_clz, "getPackageInfo",
                             "(Ljava/lang/String;I)Landroid/content/pm/PackageInfo;");
    jobject package_info = env->CallObjectMethod(pack_manager, j_mid, j_package_name, 0x00000040);
    // 3.3 获取 signatures 数组
    j_clz = env->GetObjectClass(package_info);
    jfieldID j_fid = env->GetFieldID(j_clz, "signatures", "[Landroid/content/pm/Signature;");
    jobjectArray signatures = (jobjectArray) env->GetObjectField(package_info, j_fid);
    // 3.4 获取 signatures[0]
    jobject signatures_first = env->GetObjectArrayElement(signatures, 0);
    // 3.5 调用 signatures[0].toCharsString();
    j_clz = env->GetObjectClass(signatures_first);
    j_mid = env->GetMethodID(j_clz, "toCharsString", "()Ljava/lang/String;");
    jstring j_signature_str = (jstring) env->CallObjectMethod(signatures_first, j_mid);
    const char *c_signature_str = env->GetStringUTFChars(j_signature_str, NULL);
    // 4. 比对签名是否一样
    if (strcmp(c_signature_str, APP_SIGNATURE) != 0) {
        return;
    }
    __android_log_print(ANDROID_LOG_ERROR, "JNI_TAG", "签名校验成功: %s", c_signature_str);
    // 签名认证成功
    is_verify = 1;
}