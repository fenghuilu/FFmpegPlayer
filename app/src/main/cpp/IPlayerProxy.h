//
// Created by Chaofeng.Li on 2022/11/9.
//

#ifndef FFMPEGPLAYER_IPLAYERPROXY_H
#define FFMPEGPLAYER_IPLAYERPROXY_H

#include "IPlayer.h"

#include <mutex>

class IPlayerProxy : public IPlayer {
public:
    static IPlayerProxy *get() {
        static IPlayerProxy proxy;
        return &proxy;
    }

    void init(void *vm = 0);

    virtual bool open(const char *path);

    virtual void close();

    virtual bool start();

    virtual bool isPause();

    virtual void pause(bool pause);

    virtual void initView(void *win);

    virtual double getProgress();

    virtual bool seek(double pos);

protected:
    IPlayer *player = 0;
    std::mutex mux;
};


#endif //FFMPEGPLAYER_IPLAYERPROXY_H
