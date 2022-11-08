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

    virtual bool init(void *win, XTextureType type) {
        if (!win) {
            return false;
        }

        if (!XEGL::get()->init(win)) {
            return false;
        }
        sh.init((XShaderType) type);
        textureType = type;
        return true;
    }

    virtual void draw(unsigned char *data[], int width, int height) {
        LOGD("XTexture draw");
        sh.getTexture(0, width, height, data[0]);//Y
        if (textureType == XTEXTURE_YUV420P) {
            sh.getTexture(1, width / 2, height / 2, data[1]);//U
            sh.getTexture(2, width / 2, height / 2, data[2]);//V
        } else {
            sh.getTexture(1, width / 2, height / 2, data[1], true);//UV
        }
        sh.draw();
        XEGL::get()->draw();
    }
};

XTexture *XTexture::create() {
    return new CXTexture();
}