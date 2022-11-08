//
// Created by Chaofeng.Li on 2022/9/30.
//

#ifndef FFMPEGPLAYER_FFRESAMPLE_H
#define FFMPEGPLAYER_FFRESAMPLE_H

#include "IResample.h"

struct SwrContext;

class FFResample : public IResample {
    virtual bool open(XParameter in, XParameter out = XParameter());
    virtual XData resample(XData indata);


protected :
    SwrContext *actx = 0;
};


#endif //FFMPEGPLAYER_FFRESAMPLE_H
