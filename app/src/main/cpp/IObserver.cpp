//
// Created by chaofeng.li on 2022/1/26.
//

#include "IObserver.h"

void IObserver::addObs(IObserver *obs) {
    if (!obs)return;
    mux.lock();
    obss.push_back(obs);
    mux.unlock();
}

//通知所有观察者
void IObserver::notify(XData data) {
    mux.lock();
    for (int i = 0; i < obss.size(); ++i) {
        obss.at(i)->update(data);
    }
    mux.unlock();
}