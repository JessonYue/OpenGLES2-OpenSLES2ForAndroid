//
// Created by 吴跃 on 2019/1/7.
//

#include "audio_pcm.h"
#include "malloc.h"
#include "string.h"

audio_pcm::audio_pcm(char *data, int size) {
    this->data = (char *) malloc(size);
    this->size = size;
    memcpy(this->data, data, size);
}


audio_pcm::~audio_pcm() {

}

int audio_pcm::getSize() {
    return size;
}

char *audio_pcm::getData() {
    return data;
}