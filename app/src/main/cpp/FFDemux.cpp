//
// Created by chaofeng.li on 2022/1/11.
//
extern "C" {
#include <libavformat/avformat.h>
}

#include "FFDemux.h"

//分数转为浮点数
static double r2d(AVRational r) {
    return r.num == 0 || r.den == 0 ? 0. : (double) r.num / (double) r.den;
}

void FFDemux::close() {
    mux.lock();
    if (avFormatContext) {
        avformat_close_input(&avFormatContext);
    }
    mux.unlock();
}

//打开文件或者流媒体 rtsp rmtp http
bool FFDemux::open(const char *url) {
    LOGD("open %s", url);
    close();
    mux.lock();
    int res = avformat_open_input(&avFormatContext, url, 0, 0);
    if (res != 0) {
        char buf[1024] = {0};
        av_strerror(res, buf, sizeof(buf));
        LOGE("avformat_open_input %s failed %s ", url, av_err2str(res));
        mux.unlock();
        return false;
    }
    LOGD("open %s success ", url);
    res = avformat_find_stream_info(avFormatContext, 0);
    if (res != 0) {
        char buf[1024] = {0};
        av_strerror(res, buf, sizeof(buf));
        LOGE("avformat_find_stream_info %s failed %s ", url, av_err2str(res));
        mux.unlock();
        return false;
    }
    totalMs = avFormatContext->duration / (AV_TIME_BASE / 1000);
    LOGD("totalMs = %d ", totalMs);
    mux.unlock();
    getVPara();
    getAPara();
    return true;
}

//读取一帧数据，数据由调用者清理
XData FFDemux::read() {
    mux.lock();
    if (!avFormatContext) {
        mux.unlock();
        return XData();
    }
    XData data;
    AVPacket *pkt = av_packet_alloc();
    int re = av_read_frame(avFormatContext, pkt);
    if (re != 0) {
        av_packet_free(&pkt);
        mux.unlock();
        return XData();
    }
    LOGD("pkt size is %d ,pts %lld", pkt->size, pkt->pts);
    data.data = (unsigned char *) pkt;
    data.size = pkt->size;
    if (pkt->stream_index == audioStream) {
        data.isAudio = true;
    } else if (pkt->stream_index == videoStream) {
        data.isAudio = false;
    } else {
        av_packet_free(&pkt);
    }
    //转换pts
    pkt->pts = pkt->pts * 1000 * r2d(avFormatContext->streams[pkt->stream_index]->time_base);
    pkt->dts = pkt->dts * 1000 * r2d(avFormatContext->streams[pkt->stream_index]->time_base);
    data.pts = (int) pkt->pts;
//    LOGD("demux pts %d ", data.pts);
    mux.unlock();
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
    mux.lock();
    if (!avFormatContext) {
        LOGE("getVPara failed avFormatContext is null");
        mux.unlock();
        return XParameter();
    }
    int re = av_find_best_stream(avFormatContext, AVMEDIA_TYPE_VIDEO, -1, -1, 0, 0);
    if (re < 0) {
        LOGE("av_find_best_stream failed");
        mux.unlock();
        return XParameter();
    }
    videoStream = re;
    XParameter para;
    para.para = avFormatContext->streams[re]->codecpar;
    mux.unlock();
    return para;
}

XParameter FFDemux::getAPara() {
    mux.lock();
    if (!avFormatContext) {
        LOGE("getAPara failed avFormatContext is null");
        mux.unlock();
        return XParameter();
    }
    int re = av_find_best_stream(avFormatContext, AVMEDIA_TYPE_AUDIO, -1, -1, 0, 0);
    if (re < 0) {
        LOGE("av_find_best_stream failed");
        mux.unlock();
        return XParameter();
    }
    audioStream = re;
    XParameter para;
    para.para = avFormatContext->streams[re]->codecpar;
    para.channels = para.para->channels;
    para.sample_rate = para.para->sample_rate;
    mux.unlock();
    return para;
}