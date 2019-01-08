//
// Created by 吴跃 on 2019/1/7.
//

#ifndef OPENSL_PROJECT_AUDIO_PCM_H
#define OPENSL_PROJECT_AUDIO_PCM_H


class audio_pcm {
public:
    char *data;
    int size;

public:
    audio_pcm(char* data, int size);
    ~audio_pcm();
    int getSize();
    char* getData();
};


#endif //OPENSL_PROJECT_AUDIO_PCM_H
