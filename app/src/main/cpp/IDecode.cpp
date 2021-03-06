//
// Created by chaofeng.li on 2022/1/29.
//

#include "IDecode.h"
#include "LogCommon.h"

void IDecode::run() {
    while (!isExit) {
        pktsmutex.lock();
        if (pkts.empty()) {
            pktsmutex.unlock();
            XSleep(1);
            continue;
        }
        XData pack = pkts.front();
        pkts.pop_front();
        if (this->sendPacket(pack)) {
            //一个数据包可能解码多个
            while (!isExit) {
                XData frame = this->recvFrame();
                if (!frame.data) {
                    break;
                }
                if(frame.isAudio){
                    LOGD("recv Audio Frame size is %d",frame.size);
                } else {
                    LOGD("recv video Frame size is %d",frame.size);
                }
                this->notify(frame);
            }
        }
        pack.drop();
        pktsmutex.unlock();
    }
}

void IDecode::update(XData pkt) {
    if (pkt.isAudio != isAudio) {
        return;
    }
    while (!isExit) {
        pktsmutex.lock();
        if (pkts.size() < maxList) {
            pkts.push_back(pkt);
            pktsmutex.unlock();
            break;
        }
        pktsmutex.unlock();
        XSleep(1);
    }
}