#include <jni.h>
#include <string>
#include <android/native_window_jni.h>
#include "FFDemux.h"
#include "IDecode.h"
#include "FFDecode.h"
#include "XEGL.h"

#define LOG_TAG "ffmpegplayer"

class g;

class g;

extern "C" JNIEXPORT jstring JNICALL
Java_com_feng_ffmpegplayer_MainActivity_stringFromJNI(
        JNIEnv *env,
        jobject /* this */) {
    std::string hello = "Hello from C++";
    IDemux *demux = new FFDemux();
    IDecode *vdecode = new FFDecode();
    IDecode *adecode = new FFDecode();

    demux->addObs(vdecode);
    demux->addObs(adecode);

    demux->open("/storage/emulated/0/Android/data/com.feng.ffmpegplayer/cache/test.mp4");
    vdecode->open(demux->getVPara());
    adecode->open(demux->getAPara());

    demux->start();
    vdecode->start();
    adecode->start();
//    XSleep(3000);
//    de->stop();
    return env->NewStringUTF(hello.c_str());
}
extern "C"
JNIEXPORT void JNICALL
Java_com_feng_ffmpegplayer_XPlay_initView(JNIEnv *env, jobject thiz, jobject surface) {
    // TODO: implement initView()
    ANativeWindow *win = ANativeWindow_fromSurface(env, surface);
    XEGL::get()->init(win);
}extern "C"
JNIEXPORT jstring JNICALL
Java_com_feng_opengldemo_MainActivity_stringFromJNI(JNIEnv *env, jobject thiz) {
    // TODO: implement stringFromJNI()
}