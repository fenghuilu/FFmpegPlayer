//
// Created by Chaofeng.Li on 2022/11/9.
//

#include "IPlayer.h"
#include "IDecode.h"
#include "IDemux.h"
#include "IAudioPlay.h"
#include "XParameter.h"
#include "IResample.h"
#include "IVideoView.h"
#include "LogCommon.h"

IPlayer *IPlayer::get(unsigned char index) {
    static IPlayer p[256];
    return &p[index];
}

void IPlayer::run() {
    while (!isExit) {
        mux.lock();
        if (!audioPlayer || !vdecode) {
            mux.unlock();
            XSleep(2);
            continue;
        }
        //获取音频pts,告诉视频
        int apts = audioPlayer->pts;
        vdecode->synPts = apts;
//        LOGD("apts %d",apts);
        mux.unlock();
        XSleep(2);
    }
}

bool IPlayer::open(const char *path) {
    mux.lock();
    if (!demux || !demux->open(path)) {
        LOGD("demux failed");
        mux.unlock();
        return false;
    }
    //界面有可能不需要，原始数据
    if (!vdecode || !vdecode->open(demux->getVPara(), isHardDecode)) {
        LOGD("vdecode open failed");
//        return false;
    }
    if (!adecode || !adecode->open(demux->getAPara())) {
        LOGD("adecode open failed");
//        return false;
    }
    if (outPara.sample_rate <= 0) {
        outPara = demux->getAPara();
    }
    if (!resample || resample->open(demux->getAPara(), outPara)) {
        LOGD("resample open failed");
    }
    mux.unlock();
    return true;
}

bool IPlayer::start() {
    mux.lock();
    if (!demux || !demux->start()) {
        mux.unlock();
        LOGE("demux start false");
        return false;
    }
    if (!adecode || !adecode->start()) {
        LOGE("demux start false");
    }
    if (!audioPlayer || !audioPlayer->startPlay(outPara)) {
        LOGE("demux start false");
    }
    if (!vdecode || !vdecode->start()) {
        LOGE("demux start false");
    }
    XThread::start();
    mux.unlock();
    return true;
}

void IPlayer::initView(void *win) {
    if (videoView) {
        videoView->setRender(win);
    }
}

