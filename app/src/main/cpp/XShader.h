//
// Created by chaofeng.li on 2022/2/7.
//

#ifndef FFMPEGPLAYER_XSHADER_H
#define FFMPEGPLAYER_XSHADER_H

#include <mutex>
enum XShaderType {
    XSHADER_YUV420P = 0,
    XSHADER_NV12 = 25,
    XSHADER_NV21 = 26

};

class XShader {
public:
    virtual bool init(XShaderType type = XSHADER_YUV420P);

    virtual void close();

    //获取材质并映射到内存
    virtual void getTexture(unsigned int index, int width, int height, unsigned char *buff,
                            bool isAlphe = false);

    virtual void draw();

protected:
    unsigned int vsh = 0;
    unsigned int fsh = 0;
    unsigned int program = 0;
    unsigned int texts[100] = {0};
    std::mutex mux;
};


#endif //FFMPEGPLAYER_XSHADER_H
