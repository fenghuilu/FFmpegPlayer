//
// Created by Chaofeng.Li on 2022/11/9.
//

#ifndef FFMPEGPLAYER_IPLAYER_H
#define FFMPEGPLAYER_IPLAYER_H

#include "XThread.h"
#include "XParameter.h"
#include <mutex>

class IDemux;

class IDecode;

class IResample;

class IVideoView;

class IAudioPlay;

class IPlayer : public XThread {
public:
    static IPlayer *get(unsigned char index = 0);

    virtual bool open(const char *path);

    virtual void close();

    virtual bool start();

    virtual void initView(void *win);

    //获取当前进度0.0~1.0
    virtual double getProgress();

    virtual bool seek(double pos);

    virtual void pause(bool pause);

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
    //用作音视频同步
    void run();

    std::mutex mux;

    IPlayer() {};
};


#endif //FFMPEGPLAYER_IPLAYER_H
