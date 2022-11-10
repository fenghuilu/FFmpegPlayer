//
// Created by chaofeng.li on 2022/2/7.
//

#include "XTexture.h"
#include "LogCommon.h"
#include "XEGL.h"
#include "XShader.h"

class CXTexture : public XTexture {
public:
    XShader sh;
    XTextureType textureType;
    std::mutex mux;

    virtual void drop() {
        mux.lock();
        XEGL::get()->close();
        sh.close();
        mux.unlock();
        delete this;
    }

    virtual bool init(void *win, XTextureType type) {
        mux.lock();
        XEGL::get()->close();
        sh.close();
        if (!win) {
            mux.unlock();
            return false;
        }

        if (!XEGL::get()->init(win)) {
            mux.unlock();
            return false;
        }
        sh.init((XShaderType) type);
        textureType = type;
        mux.unlock();
        return true;
    }

    virtual void draw(unsigned char *data[], int width, int height) {
        LOGD("XTexture draw");
        mux.lock();
        sh.getTexture(0, width, height, data[0]);//Y
        if (textureType == XTEXTURE_YUV420P) {
            sh.getTexture(1, width / 2, height / 2, data[1]);//U
            sh.getTexture(2, width / 2, height / 2, data[2]);//V
        } else {
            sh.getTexture(1, width / 2, height / 2, data[1], true);//UV
        }
        sh.draw();
        XEGL::get()->draw();
        mux.unlock();
    }
};

XTexture *XTexture::create() {
    return new CXTexture();
}