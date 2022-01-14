#include <jni.h>
#include <string>
#include "FFDemux.h"
#define LOG_TAG "ffmpegplayer"
extern "C" JNIEXPORT jstring JNICALL
Java_com_feng_ffmpegplayer_MainActivity_stringFromJNI(
        JNIEnv* env,
        jobject /* this */) {
    std::string hello = "Hello from C++";
    IDemux *de = new FFDemux();
    de->open("/storage/emulated/0/Android/data/com.feng.ffmpegplayer/cache/test.mp4");
    de->start();
    return env->NewStringUTF(hello.c_str());
}