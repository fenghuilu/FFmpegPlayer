//
// Created by chaofeng.li on 2022/1/29.
//

#ifndef FFMPEGPLAYER_IDECODE_H
#define FFMPEGPLAYER_IDECODE_H


#include "XParameter.h"
#include "XData.h"
#include "IObserver.h"
#include <list>

#define LOG_TAG "IDecode"

class IDecode : public IObserver {
public:
    virtual bool open(XParameter parameter, bool isHard = false) = 0;
    virtual void close() = 0;

    //future模型 发送数据到线程解码
    virtual bool sendPacket(XData pkt) = 0;

    //从线程中获取解码结果 再次调用会复用上次空间，线程不安全
    virtual XData recvFrame() = 0;

    virtual void update(XData pkt);

    bool isAudio = false;
    //最大队列缓存
    int maxList = 100;
    //同步时间，再次打开文件要清理
    int synPts = 0;
    int pts = 0;
protected:
    virtual void run();

    //缓存
    std::list<XData> pkts;
    std::mutex pktsmutex;
};


#endif //FFMPEGPLAYER_IDECODE_H
