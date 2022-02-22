//
// Created by chaofeng.li on 2022/2/7.
//

#ifndef FFMPEGPLAYER_XTEXTURE_H
#define FFMPEGPLAYER_XTEXTURE_H


class XTexture {
public:
    static XTexture *create();
    virtual bool init(void *win) = 0;
    virtual void draw(unsigned char *data[],int width,int height) = 0;

};


#endif //FFMPEGPLAYER_XTEXTURE_H
