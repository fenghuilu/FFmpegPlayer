//
// Created by Chaofeng.Li on 2022/11/8.
//
#include "SLAudioPlay.h"
#include <SLES/OpenSLES.h>
#include <SLES/OpenSLES_Android.h>

static SLObjectItf engineSL = NULL;
static SLEngineItf eng = NULL;
static SLObjectItf mix = NULL;
static SLObjectItf player = NULL;
static SLPlayItf iplayer = NULL;
static SLAndroidSimpleBufferQueueItf pcmQue = NULL;

SLAudioPlay::SLAudioPlay() {
    buf = new unsigned char[1024 * 1024];
}

SLAudioPlay::~SLAudioPlay() {
    delete buf;
    buf = 0;
}

static SLEngineItf createSL() {
    SLresult re;
    SLEngineItf en;
    re = slCreateEngine(&engineSL, 0, 0, 0, 0, 0);
    if (re != SL_RESULT_SUCCESS)return NULL;
    re = (*engineSL)->Realize(engineSL, SL_BOOLEAN_FALSE);
    if (re != SL_RESULT_SUCCESS)return NULL;
    re = (*engineSL)->GetInterface(engineSL, SL_IID_ENGINE, &en);
    if (re != SL_RESULT_SUCCESS)return NULL;
    return en;

}

void SLAudioPlay::playCall(void *bufq) {
    if (!bufq)
        return;
    SLAndroidSimpleBufferQueueItf bf = (SLAndroidSimpleBufferQueueItf) bufq;
    LOGD("SLAudioPlay::playCall");
    //阻塞
    XData data = getData();
    if (data.size <= 0) {
        LOGE("getData size is 0");
        return;
    }
    if (!buf) {
        return;
    }
    memcpy(buf, data.data, data.size);
    mux.lock();
    if (pcmQue && (*pcmQue)) {
        (*pcmQue)->Enqueue(pcmQue, data.data, data.size);
    }
    mux.unlock();
    data.drop();
}

void pcmCall(SLAndroidSimpleBufferQueueItf bf, void *context) {
    SLAudioPlay *ap = (SLAudioPlay *) context;
    if (!ap) {
        LOGE("pcmCall failed context is null");
    }
    ap->playCall((void *) bf);
}

void SLAudioPlay::close() {
    LOGD("SLAudioPlay::close");
    IAudioPlay::clear();
    mux.lock();
    //停止播放
    if (iplayer && (*iplayer)) {
        (*iplayer)->SetPlayState(iplayer, SL_PLAYSTATE_STOPPED);
    }
    LOGD("SLAudioPlay::close 1");

    //清理播放队列
    if (pcmQue && (*pcmQue)) {
        (*pcmQue)->Clear(pcmQue);
    }
    //销毁播放器对象
    if (player && (*player)) {
        (*player)->Destroy(player);
    }
    //销毁混音器
    if (mix && (*mix)) {
        (*mix)->Destroy(mix);
    }
    //销毁播放引擎
    if (engineSL && (*engineSL)) {
        (*engineSL)->Destroy(engineSL);
    }
    engineSL = NULL;
    eng = NULL;
    mix = NULL;
    player = NULL;
    iplayer = NULL;
    pcmQue = NULL;
    mux.unlock();
    LOGD("SLAudioPlay::close SUCCESS");
}

bool SLAudioPlay::startPlay(XParameter parameter) {
    close();
    LOGD("SLAudioPlay::startPlay SUCCESS");
    mux.lock();
    //创建引擎
    eng = createSL();
    if (eng) {
        LOGD("createSL success");
    } else {
        LOGE("createSL failed");
        mux.unlock();
        return false;
    }
    //创建混音器
    SLObjectItf mix = NULL;
    SLresult re = 0;
    re = (*eng)->CreateOutputMix(eng, &mix, 0, 0, 0);
    if (re != SL_RESULT_SUCCESS) {
        LOGE("CreateOutputMix FAILED");
        mux.unlock();
        return false;
    }
    re = (*mix)->Realize(mix, SL_BOOLEAN_FALSE);
    if (re != SL_RESULT_SUCCESS) {
        LOGE("Realize FAILED");
        mux.unlock();
        return false;
    }
    SLDataLocator_OutputMix outputMix = {SL_DATALOCATOR_OUTPUTMIX, mix};
    SLDataSink audioSink = {&outputMix, 0};
    //配置音频信息
    SLDataLocator_AndroidSimpleBufferQueue queue = {SL_DATALOCATOR_ANDROIDSIMPLEBUFFERQUEUE, 10};
    SLDataFormat_PCM pcm = {
            SL_DATAFORMAT_PCM,
            (SLuint32) parameter.channels,
            (SLuint32) parameter.sample_rate * 1000,
            SL_PCMSAMPLEFORMAT_FIXED_16,
            SL_PCMSAMPLEFORMAT_FIXED_16,
            SL_SPEAKER_FRONT_LEFT | SL_SPEAKER_FRONT_RIGHT,
            SL_BYTEORDER_LITTLEENDIAN
    };
    SLDataSource ds = {&queue, &pcm};

    //创建播放器
    const SLInterfaceID ids[] = {SL_IID_BUFFERQUEUE};
    const SLboolean req[] = {SL_BOOLEAN_TRUE};
    re = (*eng)->CreateAudioPlayer(eng, &player, &ds, &audioSink,
                                   sizeof(ids) / sizeof(SLInterfaceID), ids, req);
    if (re != SL_RESULT_SUCCESS) {
        LOGE("CreateAudioPlayer failed ");
        mux.unlock();
        return false;
    } else {
        LOGD("CreateAudioPlayer success");
    }
    (*player)->Realize(player, SL_BOOLEAN_FALSE);
    re = (*player)->GetInterface(player, SL_IID_PLAY, &iplayer);
    if (re != SL_RESULT_SUCCESS) {
        LOGE("GetInterface SL_IID_PLAY failed ");
        mux.unlock();
        return false;
    }
    re = (*player)->GetInterface(player, SL_IID_BUFFERQUEUE, &pcmQue);
    if (re != SL_RESULT_SUCCESS) {
        LOGE("GetInterface SL_IID_BUFFERQUEUE failed ");
        mux.unlock();
        return false;
    }
    //设置回调函数，播放队列空调用
    (*pcmQue)->RegisterCallback(pcmQue, pcmCall, this);
    //设置为播放状态
    (*iplayer)->SetPlayState(iplayer, SL_PLAYSTATE_PLAYING);
//启动队列回调
    (*pcmQue)->Enqueue(pcmQue, "", 1);
    isExit = false;
    LOGD("SLAudioPlay::startPlay success");
    mux.unlock();
    return true;

}

