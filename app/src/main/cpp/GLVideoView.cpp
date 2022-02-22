//
// Created by chaofeng.li on 2022/2/7.
//

#include "GLVideoView.h"
#include "XTexture.h"
#include "LogCommon.h"

void GLVideoView::setRender(void *win) {
    view = win;
}

void GLVideoView::render(XData data) {
    LOGD("GLVideoView render");
    if (!view) {
        LOGE("render view is null");
        return;
    }
    if (!texture) {
        texture = XTexture::create();
    }
    texture->init(view);
    texture->draw(data.datas,data.width,data.height);
}