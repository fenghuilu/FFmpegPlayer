#include <jni.h>
#include <string>
#include <android/native_window_jni.h>
#include "FFDemux.h"
#include "IDecode.h"
#include "FFDecode.h"
#include "FFResample.h"
#include "XEGL.h"
#include "XShader.h"
#include "IVideoView.h"
#include "GLVideoView.h"
#include "IAudioPlay.h"
#include "SLAudioPlay.h"

#define LOG_TAG "ffmpegplayer_tag"
IVideoView *view;
extern "C" JNIEXPORT jint JNI_OnLoad(JavaVM *vm,void *res){
    FFDecode::initHard(vm);
    return JNI_VERSION_1_4;
}
extern "C" JNIEXPORT jstring JNICALL
Java_com_feng_ffmpegplayer_MainActivity_open(
        JNIEnv *env,
        jobject thiz, jstring url_) {
    LOGD("open");
    const char *url = env->GetStringUTFChars(url_, JNI_FALSE);
    IDemux *demux = new FFDemux();
    IDecode *vdecode = new FFDecode();
    IDecode *adecode = new FFDecode();
    view = new GLVideoView();

    demux->addObs(vdecode);
    demux->addObs(adecode);
    vdecode->addObs(view);

    IResample *resample = new FFResample();
    adecode->addObs(resample);

    demux->open(url);
    vdecode->open(demux->getVPara(),true);
    adecode->open(demux->getAPara());
    XParameter outPara = demux->getAPara();
    resample->open(demux->getAPara(), outPara);
    resample->open(demux->getAPara());
    IAudioPlay *audioPlay = new SLAudioPlay();
    audioPlay->startPlay(outPara);
    resample->addObs(audioPlay);

    demux->start();
    vdecode->start();
    adecode->start();

    env->ReleaseStringUTFChars(url_, url);

    std::string hello = "Hello from C++";
    return env->NewStringUTF(hello.c_str());
}
extern "C"
JNIEXPORT void JNICALL
Java_com_feng_ffmpegplayer_XPlay_initView(JNIEnv *env, jobject thiz, jobject surface) {
    // TODO: implement initView()
    LOGD("initView");
    ANativeWindow *win = ANativeWindow_fromSurface(env, surface);
    view->setRender(win);
}