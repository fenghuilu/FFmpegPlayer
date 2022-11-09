//
// Created by chaofeng.li on 2022/1/11.
//

#ifndef FFMPEGPLAYER_IDEMUX_H
#define FFMPEGPLAYER_IDEMUX_H

#include "XData.h"
#include "XThread.h"
#include "IObserver.h"
#include "XParameter.h"

class IDemux : public IObserver {
public:
    int totalMs = 0;

    //打开文件或者流媒体 rtsp rmtp http
    virtual bool open(const char *url) =0;
    virtual void close() = 0;

    //读取一帧数据，数据由调用者清理
    virtual XData read() = 0;

    virtual XParameter getVPara() = 0;

    virtual XParameter getAPara() = 0;

    //读取一帧数据，数据由调用者清理

protected:
    void run();
};


#endif //FFMPEGPLAYER_IDEMUX_H
