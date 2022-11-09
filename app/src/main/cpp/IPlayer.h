//
// Created by Chaofeng.Li on 2022/11/9.
//

#ifndef FFMPEGPLAYER_IPLAYER_H
#define FFMPEGPLAYER_IPLAYER_H

#include "XThread.h"
#include "XParameter.h"

class IDemux;

class IDecode;

class IResample;

class IVideoView;

class IAudioPlay;

class IPlayer : public XThread {
public:
    static IPlayer *get(unsigned char index = 0);

    virtual bool open(const char *path);

    virtual bool start();

    virtual void initView(void *win);
    //是否视频硬解码
    bool isHardDecode = true;

    IDemux *demux = 0;
    IDecode *vdecode = 0;
    IDecode *adecode = 0;
    IResample *resample = 0;
    IVideoView *videoView = 0;
    IAudioPlay *audioPlayer = 0;
    //音频输出参数配置
    XParameter outPara;
protected:

    IPlayer() {};
};


#endif //FFMPEGPLAYER_IPLAYER_H