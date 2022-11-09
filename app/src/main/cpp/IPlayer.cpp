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


bool IPlayer::open(const char *path) {
    if (!demux || !demux->open(path)) {
        LOGD("demux failed");
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
    return true;
}

bool IPlayer::start() {
    if (!demux || !demux->start()) {
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
    return true;
}

void IPlayer::initView(void *win) {
    if (videoView) {
        videoView->setRender(win);
    }
}

