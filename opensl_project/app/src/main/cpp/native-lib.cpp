#include <jni.h>
#include <android/log.h>

// for opensles
#include <SLES/OpenSLES.h>
#include <SLES/OpenSLES_Android.h>

// for native asset manager
#include <sys/types.h>
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>

#include <stdio.h>
#include <malloc.h>
#include "audio_pcm.h"
#include "AudioQueue.h"

//打印日志
#include <android/log.h>
#include <zconf.h>

#define LOGI(FORMAT,...) __android_log_print(ANDROID_LOG_INFO,"jesson",FORMAT,##__VA_ARGS__);
#define LOGE(FORMAT,...) __android_log_print(ANDROID_LOG_ERROR,"jesson",FORMAT,##__VA_ARGS__);


// 引擎接口
SLObjectItf engineObject = NULL;
SLEngineItf engineEngine = NULL;

//混音器
SLObjectItf outputMixObject = NULL;
SLEnvironmentalReverbItf outputMixEnvironmentalReverb = NULL;
SLEnvironmentalReverbSettings reverbSettings = SL_I3DL2_ENVIRONMENT_PRESET_STONECORRIDOR;
SLEffectSendItf sendItf = NULL;

//assets播放器
SLObjectItf fdPlayerObject = NULL;
SLPlayItf fdPlayerPlay = NULL;
SLVolumeItf fdPlayerVolume = NULL; //声音控制接口

//uri播放器
SLObjectItf uriPlayerObject = NULL;
SLPlayItf uriPlayerPlay = NULL;
SLVolumeItf uriPlayerVolume = NULL;

//pcm
SLObjectItf pcmPlayerObject = NULL;
SLPlayItf pcmPlayerPlay = NULL;
SLVolumeItf pcmPlayerVolume = NULL;

//缓冲器队列接口
SLAndroidSimpleBufferQueueItf pcmBufferQueue;

FILE *pcmFile;
void *buffer;

uint8_t *out_buffer;

void release();

void createEngine()
{
    SLresult result;
    //创建Audio Engine
    result = slCreateEngine(&engineObject, 0, NULL, 0, NULL, NULL);
    //初始化上一步得到的openSLEngine
    result = (*engineObject)->Realize(engineObject, SL_BOOLEAN_FALSE);
    //获取SLEngine接口对象，后续的操作将使用这个对象
    result = (*engineObject)->GetInterface(engineObject, SL_IID_ENGINE, &engineEngine);
}

extern "C"
JNIEXPORT void JNICALL
Java_learning_jesson_com_opensl_1project_MainActivity_playAudioByOpenSL_1assets(JNIEnv *env, jobject instance, jobject assetManager, jstring filename) {

    release();
    const char *utf8 = env->GetStringUTFChars(filename, NULL);

    // use asset manager to open asset by filename
    AAssetManager* mgr = AAssetManager_fromJava(env, assetManager);
    AAsset* asset = AAssetManager_open(mgr, utf8, AASSET_MODE_UNKNOWN);
    env->ReleaseStringUTFChars(filename, utf8);

    // open asset as file descriptor
    off_t start, length;
    int fd = AAsset_openFileDescriptor(asset, &start, &length);
    AAsset_close(asset);

    SLresult result;


    //第一步，创建引擎
    createEngine();

    //第二步，创建混音器（打开音频输出设备）
    const SLInterfaceID mids[1] = {SL_IID_ENVIRONMENTALREVERB};
    const SLboolean mreq[1] = {SL_BOOLEAN_FALSE};
    result = (*engineEngine)->CreateOutputMix(engineEngine, &outputMixObject, 1, mids, mreq);
    (void)result;
    result = (*outputMixObject)->Realize(outputMixObject, SL_BOOLEAN_FALSE);
    (void)result;
    result = (*outputMixObject)->GetInterface(outputMixObject, SL_IID_ENVIRONMENTALREVERB, &outputMixEnvironmentalReverb);
    if (SL_RESULT_SUCCESS == result) {
        result = (*outputMixEnvironmentalReverb)->SetEnvironmentalReverbProperties(outputMixEnvironmentalReverb, &reverbSettings);
        (void)result;
    }
    //第三步，设置播放器参数和创建播放器
    // 1、 配置 audio source
    SLDataLocator_AndroidFD loc_fd = {SL_DATALOCATOR_ANDROIDFD, fd, start, length};
    SLDataFormat_MIME format_mime = {SL_DATAFORMAT_MIME, NULL, SL_CONTAINERTYPE_UNSPECIFIED};
    SLDataSource audioSrc = {&loc_fd, &format_mime};

    // 2、 配置 audio sink
    SLDataLocator_OutputMix loc_outmix = {SL_DATALOCATOR_OUTPUTMIX, outputMixObject};
    SLDataSink audioSnk = {&loc_outmix, NULL};

    // 创建播放器
    const SLInterfaceID ids[3] = {SL_IID_SEEK, SL_IID_MUTESOLO, SL_IID_VOLUME};
    const SLboolean req[3] = {SL_BOOLEAN_TRUE, SL_BOOLEAN_TRUE, SL_BOOLEAN_TRUE};
    result = (*engineEngine)->CreateAudioPlayer(engineEngine, &fdPlayerObject, &audioSrc, &audioSnk, 3, ids, req);
    (void)result;

    // 实现播放器
    result = (*fdPlayerObject)->Realize(fdPlayerObject, SL_BOOLEAN_FALSE);
    (void)result;

    // 得到播放器接口
    result = (*fdPlayerObject)->GetInterface(fdPlayerObject, SL_IID_PLAY, &fdPlayerPlay);
    (void)result;

    // 得到声音控制接口
    result = (*fdPlayerObject)->GetInterface(fdPlayerObject, SL_IID_VOLUME, &fdPlayerVolume);
    (void)result;

    // 设置播放状态
    if (NULL != fdPlayerPlay) {

        result = (*fdPlayerPlay)->SetPlayState(fdPlayerPlay, SL_PLAYSTATE_PLAYING);
        (void)result;
    }

    //设置播放音量 （100 * -50：静音 ）
    (*fdPlayerVolume)->SetVolumeLevel(fdPlayerVolume, 20 * -50);

}

extern "C"
JNIEXPORT void JNICALL
Java_learning_jesson_com_opensl_1project_MainActivity_playAudioByOpenSL_1uri(JNIEnv *env, jobject instance, jstring uri) {
    //SLresult result;
    release();
    // convert Java string to UTF-8
    const char *utf8 = env->GetStringUTFChars(uri, NULL);
    SLresult result;
    SLObjectItf engineObject;
    // create engine
    result = slCreateEngine(&engineObject, 0, NULL, 0, NULL, NULL);
    assert(SL_RESULT_SUCCESS == result);
    SLEngineItf engineEngine;
    result = (*engineObject)->Realize(engineObject, SL_BOOLEAN_FALSE);
    assert(SL_RESULT_SUCCESS == result);
    result = (*engineObject)->GetInterface(engineObject, SL_IID_ENGINE, &engineEngine);
    assert(SL_RESULT_SUCCESS == result);
    // create output mix
    SLObjectItf outputMixObject;
    result = (*engineEngine)->CreateOutputMix(engineEngine, &outputMixObject, 0, NULL, NULL);
    assert(SL_RESULT_SUCCESS == result);
    result = (*outputMixObject)->Realize(outputMixObject, SL_BOOLEAN_FALSE);
    assert(SL_RESULT_SUCCESS == result);
    // configure audio source
    SLDataLocator_URI loc_uri;
    loc_uri.locatorType = SL_DATALOCATOR_URI;
    loc_uri.URI = (SLchar *) utf8;
    SLDataFormat_MIME format_mime;
    format_mime.formatType = SL_DATAFORMAT_MIME;
    format_mime.mimeType = NULL;
    format_mime.containerType = SL_CONTAINERTYPE_UNSPECIFIED;
    SLDataSource audioSrc;
    audioSrc.pLocator = &loc_uri;
    audioSrc.pFormat = &format_mime;
    // configure audio sink
    SLDataLocator_OutputMix loc_outmix;
    loc_outmix.locatorType = SL_DATALOCATOR_OUTPUTMIX;
    loc_outmix.outputMix = outputMixObject;
    SLDataSink audioSnk;
    audioSnk.pLocator = &loc_outmix;
    audioSnk.pFormat = NULL;
    // create audio player, requesting a buffer queue interface
    SLuint32 numInterfaces = 1;
    SLInterfaceID ids[1];
    SLboolean req[1];
    ids[0] = SL_IID_BUFFERQUEUE;
    req[0] = SL_BOOLEAN_TRUE;
    SLObjectItf playerObject;
    result = (*engineEngine)->CreateAudioPlayer(engineEngine, &playerObject, &audioSrc,
                                                &audioSnk, numInterfaces, ids, req);
    assert(SL_RESULT_FEATURE_UNSUPPORTED == result);
    assert(NULL == playerObject);
#ifdef ANDROID
    ids[0] = SL_IID_ANDROIDSIMPLEBUFFERQUEUE;
    result = (*engineEngine)->CreateAudioPlayer(engineEngine, &playerObject, &audioSrc,
                                                &audioSnk, numInterfaces, ids, req);
    assert(SL_RESULT_FEATURE_UNSUPPORTED == result);
    assert(NULL == playerObject);
#endif
    // create audio player, without requesting a buffer queue interface
    result = (*engineEngine)->CreateAudioPlayer(engineEngine, &playerObject, &audioSrc,
                                                &audioSnk, 0, NULL, NULL);
    assert(SL_RESULT_SUCCESS == result);
    // realize the player
    result = (*playerObject)->Realize(playerObject, SL_BOOLEAN_FALSE);
    assert(SL_RESULT_SUCCESS == result);
    // get the play interface
    SLPlayItf playerPlay;
    result = (*playerObject)->GetInterface(playerObject, SL_IID_PLAY, &playerPlay);
    assert(SL_RESULT_SUCCESS == result);
    // get the buffer queue interface
    SLBufferQueueItf playerBufferQueue;
    result = (*playerObject)->GetInterface(playerObject, SL_IID_BUFFERQUEUE, &playerBufferQueue);
    assert(SL_RESULT_FEATURE_UNSUPPORTED == result);
    assert(NULL == playerBufferQueue);
#ifdef ANDROID
    SLAndroidSimpleBufferQueueItf playerAndroidSimpleBufferQueue;
    result = (*playerObject)->GetInterface(playerObject, SL_IID_ANDROIDSIMPLEBUFFERQUEUE,
                                           &playerAndroidSimpleBufferQueue);
    assert(SL_RESULT_FEATURE_UNSUPPORTED == result);
    assert(NULL == playerAndroidSimpleBufferQueue);
#endif
    // get the player duration
    SLmillisecond duration;
    result = (*playerPlay)->GetDuration(playerPlay, &duration);
    assert(SL_RESULT_SUCCESS == result);
    if (SL_TIME_UNKNOWN == duration)
        printf("Duration: unknown\n");
    else
        printf("Duration: %.1f\n", duration / 1000.0f);
    // set the player's state to playing
    result = (*playerPlay)->SetPlayState(playerPlay, SL_PLAYSTATE_PLAYING);
    assert(SL_RESULT_SUCCESS == result);
    // wait for the playback to finish
    for (;;) {
        SLuint32 playState;
        result = (*playerPlay)->GetPlayState(playerPlay, &playState);
        assert(SL_RESULT_SUCCESS == result);
        if (SL_PLAYSTATE_PLAYING != playState) {
            break;
        }
        usleep(10000);
    }
    // get the player duration
    result = (*playerPlay)->GetDuration(playerPlay, &duration);
    assert(SL_RESULT_SUCCESS == result);
    if (SL_TIME_UNKNOWN == duration)
        printf("Duration: unknown\n");
    else
        printf("Duration: %.1f\n", duration / 1000.0f);
    // destroy audio player
    (*playerObject)->Destroy(playerObject);
    // destroy output mix
    (*outputMixObject)->Destroy(outputMixObject);
    // destroy engine
    (*engineObject)->Destroy(engineObject);
//    //第一步，创建引擎
//    createEngine();
//
//    //第二步，创建混音器
//    const SLInterfaceID mids[2] = {SL_IID_ENVIRONMENTALREVERB,SL_IID_EFFECTSEND};
//    const SLboolean mreq[1] = {SL_BOOLEAN_FALSE};
//    result = (*engineEngine)->CreateOutputMix(engineEngine, &outputMixObject, 1, mids, mreq);
//    (void)result;
//    result = (*outputMixObject)->Realize(outputMixObject, SL_BOOLEAN_FALSE);
//    (void)result;
//    result = (*outputMixObject)->GetInterface(outputMixObject, SL_IID_ENVIRONMENTALREVERB, &outputMixEnvironmentalReverb);
//    if (SL_RESULT_SUCCESS == result) {
//        result = (*outputMixEnvironmentalReverb)->SetEnvironmentalReverbProperties(
//                outputMixEnvironmentalReverb, &reverbSettings);
//        (void)result;
//    }
//
//    result = (*outputMixObject)->GetInterface(outputMixObject, SL_IID_EFFECTSEND, &sendItf);
//
//    if (result != SL_RESULT_SUCCESS)
//        __android_log_print(ANDROID_LOG_DEBUG, "demo", "effect send not created");
//    else
//        __android_log_print(ANDROID_LOG_DEBUG, "demo", "effect send created");
//
//    //第三步，设置播放器参数和创建播放器
//    // configure audio source
//    // (requires the INTERNET permission depending on the uri parameter)
//    SLDataLocator_URI loc_uri = {SL_DATALOCATOR_URI, (SLchar *) utf8};
//    SLDataFormat_MIME format_mime = {SL_DATAFORMAT_MIME, NULL, SL_CONTAINERTYPE_UNSPECIFIED};
//    SLDataSource audioSrc = {&loc_uri, &format_mime};
//
//    // configure audio sink
//    SLDataLocator_OutputMix loc_outmix = {SL_DATALOCATOR_OUTPUTMIX, outputMixObject};
//    SLDataSink audioSnk = {&loc_outmix, NULL};
//
//    // create audio player
//    const SLInterfaceID ids[3] = {SL_IID_SEEK, SL_IID_MUTESOLO, SL_IID_VOLUME};
//    const SLboolean req[3] = {SL_BOOLEAN_TRUE, SL_BOOLEAN_TRUE, SL_BOOLEAN_TRUE};
//    result = (*engineEngine)->CreateAudioPlayer(engineEngine, &uriPlayerObject, &audioSrc, &audioSnk, 3, ids, req);
//
//    (void)result;
//
//    // release the Java string and UTF-8
//    env->ReleaseStringUTFChars(uri, utf8);
//
//    // realize the player
//    result = (*uriPlayerObject)->Realize(uriPlayerObject, SL_BOOLEAN_FALSE);
//    // this will always succeed on Android, but we check result for portability to other platforms
//    if (SL_RESULT_SUCCESS != result) {
//        (*uriPlayerObject)->Destroy(uriPlayerObject);
//        uriPlayerObject = NULL;
//        return;
//    }
//
//    // get the play interface
//    result = (*uriPlayerObject)->GetInterface(uriPlayerObject, SL_IID_PLAY, &uriPlayerPlay);
//    (void)result;
//
//
//    // get the volume interface
//    result = (*uriPlayerObject)->GetInterface(uriPlayerObject, SL_IID_VOLUME, &uriPlayerVolume);
//    (void)result;
//
//    if (NULL != uriPlayerPlay) {
//
//        // set the player's state
//        result = (*uriPlayerPlay)->SetPlayState(uriPlayerPlay, SL_PLAYSTATE_PLAYING);
//        (void)result;
//    }

    //设置播放音量 （100 * -50：静音 ）
//    (*uriPlayerVolume)->SetVolumeLevel(uriPlayerVolume, 0 * -50);
}

void release()
{

    if (pcmPlayerObject != NULL) {
        (*pcmPlayerObject)->Destroy(pcmPlayerObject);
        pcmPlayerObject = NULL;
        pcmPlayerPlay = NULL;
        pcmPlayerVolume = NULL;
        pcmBufferQueue = NULL;
        pcmFile = NULL;
        buffer = NULL;
        out_buffer = NULL;
    }

    // destroy file descriptor audio player object, and invalidate all associated interfaces
    if (fdPlayerObject != NULL) {
        (*fdPlayerObject)->Destroy(fdPlayerObject);
        fdPlayerObject = NULL;
        fdPlayerPlay = NULL;
        fdPlayerVolume = NULL;
    }

    // destroy URI audio player object, and invalidate all associated interfaces
    if (uriPlayerObject != NULL) {
        (*uriPlayerObject)->Destroy(uriPlayerObject);
        uriPlayerObject = NULL;
        uriPlayerPlay = NULL;
        uriPlayerVolume = NULL;
    }

    // destroy output mix object, and invalidate all associated interfaces
    if (outputMixObject != NULL) {
        (*outputMixObject)->Destroy(outputMixObject);
        outputMixObject = NULL;
        outputMixEnvironmentalReverb = NULL;
    }

    // destroy engine object, and invalidate all associated interfaces
    if (engineObject != NULL) {
        (*engineObject)->Destroy(engineObject);
        engineObject = NULL;
        engineEngine = NULL;
    }

}

void getPcmData(void **pcm)
{
    while(!feof(pcmFile))
    {
        fread(out_buffer, 44100 * 2 * 2, 1, pcmFile);
        if(out_buffer == NULL)
        {
            LOGI("%s", "read end");
            break;
        } else{
            LOGI("%s", "reading");
        }
        *pcm = out_buffer;
        break;
    }
}

void pcmBufferCallBack(SLAndroidSimpleBufferQueueItf bf, void * context)
{
    //assert(NULL == context);
    getPcmData(&buffer);
    // for streaming playback, replace this test by logic to find and fill the next buffer
    if (NULL != buffer) {
        SLresult result;
        // enqueue another buffer
        result = (*pcmBufferQueue)->Enqueue(pcmBufferQueue, buffer, 44100 * 2 * 2);
        // the most likely other result is SL_RESULT_BUFFER_INSUFFICIENT,
        // which for this code example would indicate a programming error
    }
}

extern "C"
JNIEXPORT void JNICALL
Java_learning_jesson_com_opensl_1project_MainActivity_playAudioByOpenSL_1pcm(JNIEnv *env, jobject instance,
                                                                 jstring pamPath_) {
    release();
    const char *pamPath = env->GetStringUTFChars(pamPath_, 0);
    pcmFile = fopen(pamPath, "r");
    if(pcmFile == NULL)
    {
        LOGE("%s", "fopen file error");
        return;
    }
    out_buffer = (uint8_t *) malloc(44100 * 2 * 2);
    SLresult result;
    // TODO
    //第一步，创建引擎
    createEngine();

    //第二步，创建混音器
    const SLInterfaceID mids[1] = {SL_IID_ENVIRONMENTALREVERB};
    const SLboolean mreq[1] = {SL_BOOLEAN_FALSE};
    result = (*engineEngine)->CreateOutputMix(engineEngine, &outputMixObject, 1, mids, mreq);
    (void)result;
    result = (*outputMixObject)->Realize(outputMixObject, SL_BOOLEAN_FALSE);
    (void)result;
    result = (*outputMixObject)->GetInterface(outputMixObject, SL_IID_ENVIRONMENTALREVERB, &outputMixEnvironmentalReverb);
    if (SL_RESULT_SUCCESS == result) {
        result = (*outputMixEnvironmentalReverb)->SetEnvironmentalReverbProperties(
                outputMixEnvironmentalReverb, &reverbSettings);
        (void)result;
    }
    SLDataLocator_OutputMix outputMix = {SL_DATALOCATOR_OUTPUTMIX, outputMixObject};
    SLDataSink audioSnk = {&outputMix, NULL};


    // 第三步，配置PCM格式信息
    SLDataLocator_AndroidSimpleBufferQueue android_queue={SL_DATALOCATOR_ANDROIDSIMPLEBUFFERQUEUE,2};
    SLDataFormat_PCM pcm={
            SL_DATAFORMAT_PCM,//播放pcm格式的数据
            2,//2个声道（立体声）
            SL_SAMPLINGRATE_44_1,//44100hz的频率
            SL_PCMSAMPLEFORMAT_FIXED_16,//位数 16位
            SL_PCMSAMPLEFORMAT_FIXED_16,//和位数一致就行
            SL_SPEAKER_FRONT_LEFT | SL_SPEAKER_FRONT_RIGHT,//立体声（前左前右）
            SL_BYTEORDER_LITTLEENDIAN//结束标志
    };
    SLDataSource slDataSource = {&android_queue, &pcm};


    const SLInterfaceID ids[3] = {SL_IID_BUFFERQUEUE, SL_IID_EFFECTSEND, SL_IID_VOLUME};
    const SLboolean req[3] = {SL_BOOLEAN_TRUE, SL_BOOLEAN_TRUE, SL_BOOLEAN_TRUE};

    result = (*engineEngine)->CreateAudioPlayer(engineEngine, &pcmPlayerObject, &slDataSource, &audioSnk, 3, ids, req);
    //初始化播放器
    (*pcmPlayerObject)->Realize(pcmPlayerObject, SL_BOOLEAN_FALSE);

//    得到接口后调用  获取Player接口
    (*pcmPlayerObject)->GetInterface(pcmPlayerObject, SL_IID_PLAY, &pcmPlayerPlay);

//    注册回调缓冲区 获取缓冲队列接口
    (*pcmPlayerObject)->GetInterface(pcmPlayerObject, SL_IID_BUFFERQUEUE, &pcmBufferQueue);
    //缓冲接口回调
    (*pcmBufferQueue)->RegisterCallback(pcmBufferQueue, pcmBufferCallBack, NULL);
//    获取音量接口
    (*pcmPlayerObject)->GetInterface(pcmPlayerObject, SL_IID_VOLUME, &pcmPlayerVolume);

//    获取播放状态接口
    (*pcmPlayerPlay)->SetPlayState(pcmPlayerPlay, SL_PLAYSTATE_PLAYING);

//    主动调用回调函数开始工作
    pcmBufferCallBack(pcmBufferQueue, NULL);

    env->ReleaseStringUTFChars(pamPath_, pamPath);
}




AudioQueue *wlQueue = NULL;
pthread_t playpcm;

void pcmBufferCallBack2(SLAndroidSimpleBufferQueueItf bf, void * context)
{
    audio_pcm * data = wlQueue->getPcmdata();
    if (NULL != data) {
        (*pcmBufferQueue)->Enqueue(pcmBufferQueue, data->getData(), data->getSize());
    }
}

void *createOpensl(void *data)
{
    SLresult result;
    // TODO
    //第一步，创建引擎
    createEngine();

    //第二步，创建混音器
    const SLInterfaceID mids[1] = {SL_IID_ENVIRONMENTALREVERB};
    const SLboolean mreq[1] = {SL_BOOLEAN_FALSE};
    result = (*engineEngine)->CreateOutputMix(engineEngine, &outputMixObject, 1, mids, mreq);
    (void)result;
    result = (*outputMixObject)->Realize(outputMixObject, SL_BOOLEAN_FALSE);
    (void)result;
    result = (*outputMixObject)->GetInterface(outputMixObject, SL_IID_ENVIRONMENTALREVERB, &outputMixEnvironmentalReverb);
    if (SL_RESULT_SUCCESS == result) {
        result = (*outputMixEnvironmentalReverb)->SetEnvironmentalReverbProperties(
                outputMixEnvironmentalReverb, &reverbSettings);
        (void)result;
    }
    SLDataLocator_OutputMix outputMix = {SL_DATALOCATOR_OUTPUTMIX, outputMixObject};
    SLDataSink audioSnk = {&outputMix, NULL};


    // 第三步，配置PCM格式信息
    SLDataLocator_AndroidSimpleBufferQueue android_queue={SL_DATALOCATOR_ANDROIDSIMPLEBUFFERQUEUE,2};
    SLDataFormat_PCM pcm={
            SL_DATAFORMAT_PCM,//播放pcm格式的数据
            2,//2个声道（立体声）
            SL_SAMPLINGRATE_44_1,//44100hz的频率
            SL_PCMSAMPLEFORMAT_FIXED_16,//位数 16位
            SL_PCMSAMPLEFORMAT_FIXED_16,//和位数一致就行
            SL_SPEAKER_FRONT_LEFT | SL_SPEAKER_FRONT_RIGHT,//立体声（前左前右）
            SL_BYTEORDER_LITTLEENDIAN//结束标志
    };
    SLDataSource slDataSource = {&android_queue, &pcm};


    const SLInterfaceID ids[3] = {SL_IID_BUFFERQUEUE, SL_IID_EFFECTSEND, SL_IID_VOLUME};
    const SLboolean req[3] = {SL_BOOLEAN_TRUE, SL_BOOLEAN_TRUE, SL_BOOLEAN_TRUE};

    result = (*engineEngine)->CreateAudioPlayer(engineEngine, &pcmPlayerObject, &slDataSource, &audioSnk, 3, ids, req);
    //初始化播放器
    (*pcmPlayerObject)->Realize(pcmPlayerObject, SL_BOOLEAN_FALSE);

//    得到接口后调用  获取Player接口
    (*pcmPlayerObject)->GetInterface(pcmPlayerObject, SL_IID_PLAY, &pcmPlayerPlay);

//    注册回调缓冲区 获取缓冲队列接口
    (*pcmPlayerObject)->GetInterface(pcmPlayerObject, SL_IID_BUFFERQUEUE, &pcmBufferQueue);
    //缓冲接口回调
    (*pcmBufferQueue)->RegisterCallback(pcmBufferQueue, pcmBufferCallBack2, NULL);
//    获取音量接口
    (*pcmPlayerObject)->GetInterface(pcmPlayerObject, SL_IID_VOLUME, &pcmPlayerVolume);

//    获取播放状态接口
    (*pcmPlayerPlay)->SetPlayState(pcmPlayerPlay, SL_PLAYSTATE_PLAYING);

//    主动调用回调函数开始工作
    pcmBufferCallBack2(pcmBufferQueue, NULL);

    pthread_exit(&playpcm);
}
