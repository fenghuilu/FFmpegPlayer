//
// Created by chaofeng.li on 2022/1/26.
//

#ifndef FFMPEGPLAYER_IOBSERVER_H
#define FFMPEGPLAYER_IOBSERVER_H


#include "XData.h"
#include "XThread.h"
#include <mutex>
#include <vector>

class IObserver : public XThread{
public:
virtual void update(XData data){}
void addObs(IObserver *obs);
//通知所有观察者
void notify(XData data);

protected:
    std::vector<IObserver *>obss;
    std::mutex mux;
};


#endif //FFMPEGPLAYER_IOBSERVER_H
