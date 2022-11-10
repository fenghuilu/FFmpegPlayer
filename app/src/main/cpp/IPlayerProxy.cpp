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

bool IPlayerProxy::open(const char *path) {
    bool re = false;
    mux.lock();
    if (player) {
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