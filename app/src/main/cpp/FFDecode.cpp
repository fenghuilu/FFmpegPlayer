//
// Created by chaofeng.li on 2022/1/29.
//
extern "C" {
#include <libavcodec/jni.h>
}

#include "FFDecode.h"
#include "LogCommon.h"

#define LOG_TAG "FFDecode" // 这个是自定义的LOG的标识

void FFDecode::initHard(void *vm) {
    av_jni_set_java_vm(vm, 0);
}

bool FFDecode::open(XParameter parameter, bool isHard) {
    LOGD("FFDecode::open  %d ", isHard);
    if (!parameter.para) {
        LOGD("!parameter.para???");
        return false;
    }
    AVCodecParameters *p = parameter.para;
    AVCodec *avCodec = avcodec_find_decoder(p->codec_id);
    if (isHard) {//so可能不支持硬件解码
        avCodec = avcodec_find_decoder_by_name("h264_mediacodec");
    }
    if (!avCodec) {
        LOGE("avcodec_find_decoder %d failed! %d ", p->codec_id, isHard);
        return false;
    }
    LOGD("avcodec_find_decoder success %d ", isHard);
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
    XData d = XData();
    d.data = (unsigned char *) avFrame;
    if (avCodecContext->codec_type == AVMEDIA_TYPE_VIDEO) {
        d.isAudio = false;
        d.size = (avFrame->linesize[0] + avFrame->linesize[1] + avFrame->linesize[2]) *
                 avFrame->height;
        d.width = avFrame->width;
        d.height = avFrame->height;
        d.format = avFrame->format;
//        LOGD("video %d %d %d %d", avFrame->linesize[0], avFrame->linesize[1], avFrame->linesize[2],
//             avFrame->height);
//        LOGD("data format is %d ", d.format);

        if (d.format == 0) {//YUV420P 需要对齐
            d.datas[0] = new unsigned char[avFrame->width * avFrame->height];
            for (int i = 0; i < avFrame->height; ++i) {
                memcpy(d.datas[0] + i * avFrame->width, avFrame->data[0] + i * avFrame->linesize[0],
                       avFrame->width);
            }

            d.datas[1] = new unsigned char[avFrame->width * avFrame->height / 4];
            for (int i = 0; i < avFrame->height / 2; ++i) {

                memcpy(d.datas[1] + i * avFrame->width / 2,
                       avFrame->data[1] + i * avFrame->linesize[1],
                       avFrame->width / 2);
            }

            d.datas[2] = new unsigned char[avFrame->width * avFrame->height / 4];
            for (int i = 0; i < avFrame->height / 2; ++i) {
                memcpy(d.datas[2] + i * avFrame->width / 2,
                       avFrame->data[2] + i * avFrame->linesize[2],
                       avFrame->width / 2);
            }
        } else {//NV21,NV12
            memcpy(d.datas,avFrame->data, sizeof(d.datas));
        }
    } else {
        //样本字节数*单通道样本数*通道数
        d.isAudio = true;
        d.size =
                av_get_bytes_per_sample((AVSampleFormat) avFrame->format) *
                avFrame->nb_samples * 2;
//        LOGD("audio %d %d ", av_get_bytes_per_sample((AVSampleFormat) avFrame->format),
//             avFrame->nb_samples);
        memcpy(d.datas, avFrame->data, sizeof(d.datas));
    }
    return d;
}