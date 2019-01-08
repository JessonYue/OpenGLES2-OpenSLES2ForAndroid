//
// Created by 吴跃 on 2019/1/7.
//

#ifndef OPENSL_PROJECT_AUDIOQUEUE_H
#define OPENSL_PROJECT_AUDIOQUEUE_H


#include "queue"
#include "audio_pcm.h"
#include "pthread.h"

class AudioQueue {

public:
    std::queue<audio_pcm*> queuePacket;
    pthread_mutex_t mutexPacket;
    pthread_cond_t condPacket;

public:
    AudioQueue();
    ~AudioQueue();
    int putPcmdata(audio_pcm *data);
    audio_pcm* getPcmdata();
    int clearPcmdata();

    void release();
    int getPcmdataSize();

    int noticeThread();
};



#endif //OPENSL_PROJECT_AUDIOQUEUE_H
