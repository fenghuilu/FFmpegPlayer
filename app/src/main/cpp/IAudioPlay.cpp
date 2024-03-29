//
// Created by Chaofeng.Li on 2022/11/8.
//

#include "IAudioPlay.h"
#include "LogCommon.h"

void IAudioPlay::clear() {
    framesMutex.lock();
    while (!frames.empty()) {
        frames.front().drop();
        frames.pop_front();
    }
    framesMutex.unlock();
}

XData IAudioPlay::getData() {
    XData data;
    isRuning = true;
    while (!isExit) {
        if (isPause()) {
            XSleep(2);
            continue;
        }
        framesMutex.lock();
        if (!frames.empty()) {//有数据的情况
            data = frames.front();
            frames.pop_front();
            framesMutex.unlock();
            pts = data.pts;
            return data;
        }
        framesMutex.unlock();
        XSleep(1);
    }
    isRuning = false;
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

