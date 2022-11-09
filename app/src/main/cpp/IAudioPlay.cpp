//
// Created by Chaofeng.Li on 2022/11/8.
//

#include "IAudioPlay.h"
#include "LogCommon.h"

XData IAudioPlay::getData() {
    XData data;
    while (!isExit){
        framesMutex.lock();
        if(!frames.empty()){//有数据的情况
            data = frames.front();
            frames.pop_front();
            framesMutex.unlock();
            pts = data.pts;
            return data;
        }
        framesMutex.unlock();
        XSleep(1);
    }
    //未获取数据异常走这里
    return data;
}

void IAudioPlay::update(XData data) {
    LOGD("IAudioPlay::update %d ", data.size);
//压入缓冲队列
    if (!data.data || data.size <= 0)return;
    while (!isExit) {
        framesMutex.lock();
        if (frames.size() > maxFrame) {
            framesMutex.unlock();
            XSleep(1);
            continue;
        }
        frames.push_back(data);
        framesMutex.unlock();
        break;
    }
}

