//
// Created by chaofeng.li on 2022/1/29.
//

#include "FFDecode.h"
#include "LogCommon.h"

bool FFDecode::open(XParameter parameter) {
    if (!parameter.para)return false;
    AVCodecParameters *p = parameter.para;
    AVCodec *avCodec = avcodec_find_decoder(p->codec_id);
    if (!avCodec) {
        LOGE("avcodec_find_decoder %d failed!", p->codec_id);
        return false;
    }
    LOGD("avcodec_find_decoder success");
    //创建解码器上下文,并复制参数
    avCodecContext = avcodec_alloc_context3(avCodec);
    avcodec_parameters_to_context(avCodecContext, p);
    avCodecContext->thread_count = 8;
    //打开解码器
    int re = avcodec_open2(avCodecContext, 0, 0);
    if (re != 0) {
        LOGE("avcodec_open2 failed %s ", av_err2str(re));
        return false;
    }
    if (avCodecContext->codec_type == AVMEDIA_TYPE_VIDEO) {
        LOGD("avcodec_open2 video success");
        this->isAudio = false;
    } else {
        LOGD("avcodec_open2 audio success");
        this->isAudio = true;
    }
    return true;
}

bool FFDecode::sendPacket(XData pkt) {
    if (pkt.size <= 0 || !pkt.data)return false;
    if (!avCodecContext) {
        return false;
    }
    int re = avcodec_send_packet(avCodecContext, (AVPacket *) pkt.data);
    if (re != 0) {
        LOGE("avcodec_send_packet failed %s ", av_err2str(re));
        return false;
    } else {
//        LOGE("avcodec_send_packet success %d ", pkt.size);
    }

    return true;
}

//从线程中获取解码结果
XData FFDecode::recvFrame() {
    if (!avCodecContext) {
        return XData();
    }
    if (!avFrame) {
        avFrame = av_frame_alloc();
    }
    int re = avcodec_receive_frame(avCodecContext, avFrame);
    if (re != 0) {
        if (re == AVERROR(EAGAIN)) {
//            LOGE("avcodec_receive_frame need send more packet");
        } else {
            LOGE("avcodec_receive_frame failed %s ", av_err2str(re));
        }
        return XData();
    } else {
//        LOGE("avcodec_receive_frame success ");
    }
    XData d;
    d.data = (unsigned char *) avFrame;
    if (avCodecContext->codec_type == AVMEDIA_TYPE_VIDEO) {
        d.isAudio = false;
        d.size = (avFrame->linesize[0] + avFrame->linesize[1] + avFrame->linesize[2]) *
                 avFrame->height;
        d.width = avFrame->width;
        d.height = avFrame->height;
        LOGD("video %d %d %d %d", avFrame->linesize[0], avFrame->linesize[1], avFrame->linesize[2],
             avFrame->height);
    } else {
        //样本字节数*单通道样本数*通道数
        d.isAudio = true;
        d.size =
                av_get_bytes_per_sample((AVSampleFormat) avFrame->format) * avFrame->nb_samples * 2;
        LOGD("audio %d %d ", av_get_bytes_per_sample((AVSampleFormat) avFrame->format),
             avFrame->nb_samples);
    }
    memcpy(d.datas, avFrame->data, sizeof(d.datas));
    return d;
}