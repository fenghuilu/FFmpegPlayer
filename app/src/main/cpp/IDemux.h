//
// Created by chaofeng.li on 2022/1/11.
//

#ifndef FFMPEGPLAYER_IDEMUX_H
#define FFMPEGPLAYER_IDEMUX_H
#include "XData.h"
#include "XThread.h"

class IDemux :public XThread{
public:
    //打开文件或者流媒体 rtsp rmtp http
    virtual bool open(const char *url);
    //读取一帧数据，数据由调用者清理
    virtual XData read();
    int totalMs = 0;
protected:
    virtual void run();
};


#endif //FFMPEGPLAYER_IDEMUX_H
