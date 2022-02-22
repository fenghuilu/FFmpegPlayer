//
// Created by chaofeng.li on 2022/2/7.
//

#ifndef FFMPEGPLAYER_XSHADER_H
#define FFMPEGPLAYER_XSHADER_H


class XShader {
public:
    virtual bool init();

    //获取材质并映射到内存
    virtual void getTexture(unsigned int index, int width, int height, unsigned char *buff);

    virtual void draw();

protected:
    unsigned int vsh = 0;
    unsigned int fsh = 0;
    unsigned int program = 0;
    unsigned int texts[100] = {0};
};


#endif //FFMPEGPLAYER_XSHADER_H
