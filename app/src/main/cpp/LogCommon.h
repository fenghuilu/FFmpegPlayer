//
// Created by feng on 2020/4/18.
//


#ifndef FFMPEGPLAYER_LOGCOMMON_H
#define FFMPEGPLAYER_LOGCOMMON_H
#include <jni.h>
#include <android/log.h>
#define LOG_TAG "COMMON_TAG" // 这个是自定义的LOG的标识
//#undef LOG // 取消默认的LOG
#define LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG, __VA_ARGS__)
#define LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG,LOG_TAG, __VA_ARGS__)
#define LOGW(...)  __android_log_print(ANDROID_LOG_WARN,LOG_TAG, __VA_ARGS__)
#define LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG, __VA_ARGS__)
#define LOGF(...)  __android_log_print(ANDROID_LOG_FATAL,LOG_TAG, __VA_ARGS__)
#endif //MEDIALEARN_LOGCOMMON_H
