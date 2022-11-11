//
// Created by Chaofeng.Li on 2022/11/9.
//

#include "IPlayerProxy.h"
#include "FFPlayerBuilder.h"

void IPlayerProxy::close() {
    mux.lock();
    if (player) {
        player->close();
    }
    mux.unlock();
}

void IPlayerProxy::init(void *vm) {
    mux.lock();
    if (vm) {
        FFPlayerBuilder::initHard(vm);
    }
    if (!player) {
        player = FFPlayerBuilder::get()->buildPlayer();
    }
    mux.unlock();
}

double IPlayerProxy::getProgress() {
    double pos = 0.0;
    mux.lock();
    if (player) {
        pos = player->getProgress();
    }
    mux.unlock();
    return pos;
}

bool IPlayerProxy::isPause() {
    bool re = false;
    mux.lock();
    if (player) {
        re = player->isPause();
    }
    mux.unlock();
    return re;
}

void IPlayerProxy::pause(bool pause) {
    mux.lock();
    if (player) {
        player->pause(pause);
    }
    mux.unlock();
}

bool IPlayerProxy::seek(double pos) {
    bool re = false;
    mux.lock();
    if (player) {
        re = player->seek(pos);
    }
    mux.unlock();
    return re;
}

bool IPlayerProxy::open(const char *path) {
    bool re = false;
    mux.lock();
    if (player) {
        player->isHardDecode = isHardDecode;
        re = player->open(path);
    }
    mux.unlock();
    return re;
}

bool IPlayerProxy::start() {
    bool re = false;
    mux.lock();
    if (player) {
        re = player->start();
    }
    mux.unlock();
    return re;
}

void IPlayerProxy::initView(void *win) {
    mux.lock();
    if (player) {
        player->initView(win);
    }
    mux.unlock();
}