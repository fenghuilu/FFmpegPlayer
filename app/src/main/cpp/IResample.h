//
// Created by Chaofeng.Li on 2022/9/30.
//

#ifndef FFMPEGPLAYER_IRESAMPLE_H
#define FFMPEGPLAYER_IRESAMPLE_H

#include "IObserver.h"
#include "XParameter.h"


class IResample : public IObserver {
public:
    virtual bool open(XParameter in, XParameter out = XParameter()) = 0;

    virtual void close() = 0;

    virtual XData resample(XData indata) = 0;

    virtual void update(XData indata);

    int outChannels = 2;
    int outFormat = 0;
};


#endif //FFMPEGPLAYER_IRESAMPLE_H
