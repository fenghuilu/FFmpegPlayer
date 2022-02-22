//
// Created by chaofeng.li on 2022/2/7.
//

#ifndef FFMPEGPLAYER_XEGL_H
#define FFMPEGPLAYER_XEGL_H


class XEGL {
public:
    virtual bool init(void *win) = 0;

    static XEGL *get();

    virtual void draw() = 0;

protected:
    XEGL() {}
};


#endif //FFMPEGPLAYER_XEGL_H
