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

void IPlayer::close() {
    mux.lock();
    //先关闭主体线程，再清理观察者
    //音视频同步线程
    XThread::stop();
    //解封装
    if (demux)
        demux->stop();
    //解码
    if (vdecode)
        vdecode->stop();
    if (adecode)
        adecode->stop();
    if (audioPlayer) {
        audioPlayer->stop();
    }
    //清理缓冲队列
    if (vdecode)
        vdecode->clear();
    if (adecode)
        adecode->clear();
    if (audioPlayer)
        audioPlayer->clear();

    //清理资源
    if (audioPlayer)
        audioPlayer->close();
    if (videoView)
        videoView->close();
    if (adecode)
        adecode->close();
    if (vdecode)
        vdecode->close();
    if (demux)
        demux->close();

    mux.unlock();
}

double IPlayer::getProgress() {
    double pos = 0.0;
    mux.lock();
    int total = 0;
    if (demux) {
        total = demux->totalMs;
    }
    if (total > 0) {
        if (vdecode) {
            pos = (double) vdecode->pts / (double) total;
        }
    }
    mux.unlock();
    return pos;
}

void IPlayer::pause(bool pause) {
    mux.lock();
    XThread::pause(pause);
    if (demux) {
        demux->pause(pause);
    }
    if (vdecode) {
        vdecode->pause(pause);
    }
    if (adecode) {
        adecode->pause(pause);
    }
    if (audioPlayer) {
        audioPlayer->pause(pause);
    }
    mux.unlock();
}

bool IPlayer::seek(double pos) {
    LOGD("IPlayer::seek %lf",pos);

    bool re = false;
    if (!demux)
        return false;
    //暂停所有线程
    pause(true);
    mux.lock();
    //清理缓冲队列
    if (vdecode) {
        vdecode->clear();
        vdecode->clear();
    }
    if (adecode) {
        adecode->clear();
    }
    if (audioPlayer) {
        audioPlayer->clear();
    }
    if (demux) {
        re = demux->seek(pos);
    }
    //解码到实际需要显示的帧
    if (!vdecode) {
        mux.unlock();
        pause(false);
        return re;
    }
    int seekPts = pos * demux->totalMs;
    while (!isExit) {
        XData pkt = demux->read();
        if (pkt.size <= 0)break;
        if (pkt.isAudio) {
            if (pkt.pts < seekPts) {
                pkt.drop();
                continue;
            }
            demux->notify(pkt);
            continue;
        }
        vdecode->sendPacket(pkt);
        pkt.drop();
        XData data = vdecode->recvFrame();
        if (data.size <= 0) {
            continue;
        }
        if (data.pts >= seekPts) {
//            vdecode->notify(data);
            break;
        }
    }
    mux.unlock();
    pause(false);
    LOGD("IPlayer::seek success");
    return re;
}

bool IPlayer::open(const char *path) {
    close();
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
//    if (outPara.sample_rate <= 0) {
    outPara = demux->getAPara();
//    }
    if (!resample || !resample->open(demux->getAPara(), outPara)) {
        LOGD("resample open failed");
    }
    mux.unlock();
    return true;
}

bool IPlayer::start() {
    LOGD("IPlayer::start()");
    mux.lock();
    if (!audioPlayer || !audioPlayer->startPlay(outPara)) {
        LOGE("demux start false");
    }
    if (!vdecode || !vdecode->start()) {
        LOGE("demux start false");
    }
    if (!adecode || !adecode->start()) {
        LOGE("demux start false");
    }
    if (!demux || !demux->start()) {
        mux.unlock();
        LOGE("demux start false");
        return false;
    }
    XThread::start();
    mux.unlock();
    LOGE("IPlayer::start() success");
    return true;
}

void IPlayer::initView(void *win) {
    if (videoView) {
        videoView->close();
        videoView->setRender(win);
    }
}

