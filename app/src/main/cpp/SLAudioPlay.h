//
// Created by Chaofeng.Li on 2022/11/8.
//
#ifndef FFMPEGPLAYER_SLAUDIOPLAY_H
#define FFMPEGPLAYER_SLAUDIOPLAY_H

#include "IAudioPlay.h"
#include "LogCommon.h"
#include <mutex>
#define LOG_TAG "SLAudioPlay" // 这个是自定义的LOG的标识


class SLAudioPlay : public IAudioPlay {
public:
    virtual bool startPlay(XParameter parameter);

    void playCall(void *bufq);

    virtual void close();
    SLAudioPlay();
    virtual ~SLAudioPlay();

protected:
    unsigned char *buf = 0;
    std::mutex mux;
};


#endif //FFMPEGPLAYER_SLAUDIOPLAY_H
