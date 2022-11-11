//
// Created by chaofeng.li on 2022/1/11.
//

#ifndef FFMPEGPLAYER_FFDEMUX_H
#define FFMPEGPLAYER_FFDEMUX_H

#include "IDemux.h"
#include "LogCommon.h"
#include <mutex>

#define LOG_TAG "FFDemux"
struct AVFormatContext;

class FFDemux : public IDemux {
public:
    //打开文件或者流媒体 rtsp rmtp http
    virtual bool open(const char *url);
    //0.0~1.0
    virtual bool seek(double pos);
    virtual void close();

    XParameter getVPara();

    XParameter getAPara();

    //读取一帧数据，数据由调用者清理
    virtual XData read();

    FFDemux();

private:
    AVFormatContext *avFormatContext = 0;
    std::mutex mux;
    int audioStream = 1;
    int videoStream = 0;
};


#endif //FFMPEGPLAYER_FFDEMUX_H
