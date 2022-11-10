//
// Created by chaofeng.li on 2022/2/7.
//

#ifndef FFMPEGPLAYER_GLVIDEOVIEW_H
#define FFMPEGPLAYER_GLVIDEOVIEW_H


#include "XData.h"
#include "IVideoView.h"

class XTexture;

class GLVideoView: public IVideoView{
public:
    virtual void setRender(void *win);

    virtual void render(XData data);
    virtual void close();
protected:
    void *view = 0;
    XTexture *texture = 0;
    std::mutex mux;
};


#endif //FFMPEGPLAYER_GLVIDEOVIEW_H
