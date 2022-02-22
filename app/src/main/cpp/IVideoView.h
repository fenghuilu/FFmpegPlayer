//
// Created by chaofeng.li on 2022/2/7.
//

#ifndef FFMPEGPLAYER_IVIDEOVIEW_H
#define FFMPEGPLAYER_IVIDEOVIEW_H


#include "XData.h"
#include "IObserver.h"

class IVideoView : public IObserver{
public:

    virtual void setRender(void *win) = 0;
    virtual void render(XData data) = 0;
    virtual void update(XData data);
};


#endif //FFMPEGPLAYER_IVIDEOVIEW_H
