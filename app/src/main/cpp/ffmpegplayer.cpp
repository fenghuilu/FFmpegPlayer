#include <jni.h>
#include <string>
#include <android/native_window_jni.h>
#include "LogCommon.h"
#include "IPlayerProxy.h"


#define LOG_TAG "ffmpegplayer_tag"
static IPlayer *player = NULL;
extern "C" JNIEXPORT jint JNI_OnLoad(JavaVM *vm, void *res) {
    LOGD("JNI_OnLoad");
    IPlayerProxy::get()->init(vm);
    return JNI_VERSION_1_4;
}
extern "C" JNIEXPORT jstring JNICALL
Java_com_feng_ffmpegplayer_MainActivity_open(
        JNIEnv *env,
        jobject thiz, jstring url_) {
    LOGD("open");
    const char *url = env->GetStringUTFChars(url_, JNI_FALSE);
    IPlayerProxy::get()->open(url);
    IPlayerProxy::get()->start();

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
    IPlayerProxy::get()->initView(win);
}