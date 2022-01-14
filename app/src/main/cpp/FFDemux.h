//
// Created by chaofeng.li on 2022/1/11.
//

#ifndef FFMPEGPLAYER_FFDEMUX_H
#define FFMPEGPLAYER_FFDEMUX_H

#include "IDemux.h"
#include "LogCommon.h"

#define LOG_TAG "FFDemux"
struct AVFormatContext;

class FFDemux : public IDemux {
public:
    //打开文件或者流媒体 rtsp rmtp http
    bool open(const char *url);

    //读取一帧数据，数据由调用者清理
    XData read();

    FFDemux();

private:
    AVFormatContext *avFormatContext = 0;
};


#endif //FFMPEGPLAYER_FFDEMUX_H
