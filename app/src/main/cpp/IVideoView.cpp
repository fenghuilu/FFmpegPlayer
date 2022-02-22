//
// Created by chaofeng.li on 2022/2/7.
//

#include "IVideoView.h"
#include "LogCommon.h"

void IVideoView::updata(XData data) {
    LOGD("IVideoView updata");
    if (!data.isAudio) {
        this->render(data);
    }
}