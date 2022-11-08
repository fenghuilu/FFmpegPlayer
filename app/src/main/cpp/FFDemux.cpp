//
// Created by chaofeng.li on 2022/1/11.
//
extern "C" {
#include <libavformat/avformat.h>
}

#include "FFDemux.h"

//打开文件或者流媒体 rtsp rmtp http
bool FFDemux::open(const char *url) {
    LOGD("open %s", url);
    int res = avformat_open_input(&avFormatContext, url, 0, 0);
    if (res != 0) {
        char buf[1024] = {0};
        av_strerror(res, buf, sizeof(buf));
        LOGE("avformat_open_input %s failed %s ", url, av_err2str(res));
        return false;
    }
    LOGD("open %s success ", url);
    res = avformat_find_stream_info(avFormatContext, 0);
    if (res != 0) {
        char buf[1024] = {0};
        av_strerror(res, buf, sizeof(buf));
        LOGE("avformat_find_stream_info %s failed %s ", url, av_err2str(res));
        return false;
    }
    totalMs = avFormatContext->duration / (AV_TIME_BASE / 1000);
    LOGD("totalMs = %d ", totalMs);
    getVPara();
    getAPara();
    return true;
}

//读取一帧数据，数据由调用者清理
XData FFDemux::read() {
    if (!avFormatContext)return XData();
    XData data;
    AVPacket *pkt = av_packet_alloc();
    int re = av_read_frame(avFormatContext, pkt);
    if (re != 0) {
        av_packet_free(&pkt);
        return XData();
    }
    LOGD("pkt size is %d ,pts %lld", pkt->size, pkt->pts);
    data.data = (unsigned char *) pkt;
    data.size = pkt->size;
    if(pkt->stream_index == audioStream){
        data.isAudio = true;
    }else if(pkt->stream_index == videoStream){
        data.isAudio = false;
    } else {
        av_packet_free(&pkt);
    }
    return data;
}

FFDemux::FFDemux() {
    static bool isFirst = true;
    if (isFirst) {
        isFirst = false;
        //注册所有封装器
        av_register_all();
        //注册所有解码器
        avcodec_register_all();
        //初始化网络
        avformat_network_init();
    }
}

XParameter FFDemux::getVPara() {
    if (!avFormatContext) {
        LOGE("getVPara failed avFormatContext is null");

        return XParameter();
    }
    int re = av_find_best_stream(avFormatContext, AVMEDIA_TYPE_VIDEO, -1, -1, 0, 0);
    if (re < 0) {
        LOGE("av_find_best_stream failed");
        return XParameter();
    }
    videoStream = re;
    XParameter para;
    para.para = avFormatContext->streams[re]->codecpar;
    return para;
}
XParameter FFDemux::getAPara() {
    if (!avFormatContext) {
        LOGE("getAPara failed avFormatContext is null");

        return XParameter();
    }
    int re = av_find_best_stream(avFormatContext, AVMEDIA_TYPE_AUDIO, -1, -1, 0, 0);
    if (re < 0) {
        LOGE("av_find_best_stream failed");
        return XParameter();
    }
    audioStream = re;
    XParameter para;
    para.para = avFormatContext->streams[re]->codecpar;
    para.channels = para.para->channels;
    para.sample_rate = para.para->sample_rate;
    return para;
}