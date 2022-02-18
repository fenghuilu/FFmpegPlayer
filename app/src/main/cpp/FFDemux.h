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
<<<<<<< HEAD
<<<<<<< HEAD
    bool open(const char *url);

    //读取一帧数据，数据由调用者清理
    XData read();
=======
    virtual bool open(const char *url);
=======
    bool open(const char *url);
>>>>>>> add gldemo

    XParameter getVPara();

    XParameter getAPara();
    //读取一帧数据，数据由调用者清理
<<<<<<< HEAD
    virtual XData read();
>>>>>>> first commit
=======
    XData read();
>>>>>>> add gldemo

    FFDemux();

private:
    AVFormatContext *avFormatContext = 0;
    int audioStream = 1;
    int videoStream = 0;
};


#endif //FFMPEGPLAYER_FFDEMUX_H
