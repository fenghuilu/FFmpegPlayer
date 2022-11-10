//
// Created by Chaofeng.Li on 2022/11/8.
//

#ifndef FFMPEGPLAYER_IAUDIOPLAY_H
#define FFMPEGPLAYER_IAUDIOPLAY_H

#include <list>
#include "IObserver.h"
#include "XParameter.h"


class IAudioPlay : public IObserver {
public:
    //缓冲满后阻塞
    virtual void update(XData data);

    //获取缓冲数据，如没有则阻塞
    virtual XData getData();

    virtual bool startPlay(XParameter parameter) = 0;

    virtual void close() = 0;

    virtual void clear();

//最大缓冲
    int maxFrame = 100;
    int pts = 0;
protected:
    std::list<XData> frames;
    std::mutex framesMutex;
};


#endif //FFMPEGPLAYER_IAUDIOPLAY_H
