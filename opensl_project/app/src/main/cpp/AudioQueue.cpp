//
// Created by 吴跃 on 2019/1/7.
//

#include "AudioQueue.h"
AudioQueue::AudioQueue() {
    pthread_mutex_init(&mutexPacket, NULL);
    pthread_cond_init(&condPacket, NULL);
}

AudioQueue::~AudioQueue() {
    pthread_mutex_destroy(&mutexPacket);
    pthread_cond_destroy(&condPacket);
}

void AudioQueue::release() {
    noticeThread();
    clearPcmdata();
}

int AudioQueue::putPcmdata(audio_pcm *data){

    pthread_mutex_lock(&mutexPacket);
    queuePacket.push(data);
    pthread_cond_signal(&condPacket);
    pthread_mutex_unlock(&mutexPacket);

    return 0;
}

audio_pcm* AudioQueue::getPcmdata(){

    pthread_mutex_lock(&mutexPacket);
    audio_pcm *pkt = NULL;
    if(queuePacket.size() > 0)
    {
        pkt = queuePacket.front();
        queuePacket.pop();
    } else{
        pthread_cond_wait(&condPacket, &mutexPacket);
    }
    pthread_mutex_unlock(&mutexPacket);
    return pkt;
}

int AudioQueue::clearPcmdata() {

    pthread_cond_signal(&condPacket);
    pthread_mutex_lock(&mutexPacket);
    while (!queuePacket.empty())
    {
        audio_pcm *pkt = queuePacket.front();
        queuePacket.pop();
        free(pkt->getData());
        pkt = NULL;
    }
    pthread_mutex_unlock(&mutexPacket);
    return 0;
}

int AudioQueue::getPcmdataSize() {
    int size = 0;
    pthread_mutex_lock(&mutexPacket);
    size = queuePacket.size();
    pthread_mutex_unlock(&mutexPacket);
    return size;
}

int AudioQueue::noticeThread() {
    pthread_cond_signal(&condPacket);
    return 0;
}
