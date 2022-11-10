//
// Created by chaofeng.li on 2022/2/7.
//

#include "GLVideoView.h"
#include "XTexture.h"
#include "LogCommon.h"

void GLVideoView::setRender(void *win) {
    view = win;
}

void GLVideoView::close() {
    mux.lock();
    if (texture) {
        texture->drop();
        texture = 0;
    }
    mux.unlock();
}

void GLVideoView::render(XData data) {
    LOGD("GLVideoView render");
    if (!view) {
        LOGE("render view is null");
        return;
    }
    if (!texture) {
        texture = XTexture::create();
        texture->init(view, (XTextureType) data.format);
    }
    texture->draw(data.datas, data.width, data.height);
}