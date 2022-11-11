//
// Created by chaofeng.li on 2022/1/29.
//

#ifndef FFMPEGPLAYER_FFDECODE_H
#define FFMPEGPLAYER_FFDECODE_H

extern "C" {
#include <libavcodec/avcodec.h>
#include <libavutil/frame.h>
}

#include "XParameter.h"
#include "IDecode.h"

struct AVCodecContext;

class FFDecode : public IDecode {
public:
    static void initHard(void *vm);

    bool open(XParameter parameter, bool isHard = false);

    void close();
    virtual void clear();
//future模型 发送数据到线程解码
    virtual bool sendPacket(XData pkt);

    //从线程中获取解码结果
    virtual XData recvFrame();

protected:
    AVCodecContext *avCodecContext = 0;
    AVFrame *avFrame = 0;
    std::mutex mux;
};


#endif //FFMPEGPLAYER_FFDECODE_H
