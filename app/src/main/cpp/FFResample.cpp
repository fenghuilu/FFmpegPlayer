//
// Created by Chaofeng.Li on 2022/9/30.
extern "C" {
#include <libswresample/swresample.h>
#include <libavcodec/avcodec.h>
}

//

#include "FFResample.h"
#include "LogCommon.h"

void FFResample::close() {
    LOGD("FFResample::close()");
    mux.lock();
    if (actx) {
        swr_free(&actx);
    }
    mux.unlock();
}

bool FFResample::open(XParameter in, XParameter out) {
    close();
    LOGD("FFResample::open()");
    mux.lock();
    if (in.para->format)
        actx = swr_alloc();
    actx = swr_alloc_set_opts(actx,
                              av_get_default_channel_layout(out.channels),
                              AV_SAMPLE_FMT_S16, out.sample_rate,
                              av_get_default_channel_layout(in.para->channels),
                              (AVSampleFormat) in.para->format, in.para->sample_rate,
                              0, 0
    );
    int re = swr_init(actx);
    if (re != 0) {
        LOGD("swr_init FAILED");
        mux.unlock();
        return false;
    } else {
        LOGD("swr_init SUCCESS");
    }
    outChannels = in.para->channels;
    outFormat = AV_SAMPLE_FMT_S16;
    mux.unlock();
    LOGD("FFResample open success");
    return true;
}

XData FFResample::resample(XData indata) {
    LOGD("resample size id %d ", indata.size);
    if (indata.size <= 0 || !indata.data) {
        return XData();
    }
    mux.lock();
    if (!actx) {
        mux.unlock();
        return XData();
    }
    AVFrame *frame = (AVFrame *) indata.data;

//输出空间分配
    XData out;
//通道数*单通道样本数*样本字节大小
    int outsize =
            outChannels * frame->nb_samples * av_get_bytes_per_sample((AVSampleFormat) outFormat);

    out.alloc(outsize);
    uint8_t *outArr[2] = {0};
    outArr[0] = out.data;
    int len = swr_convert(actx, outArr, frame->nb_samples, (const uint8_t **) frame->data,
                          frame->nb_samples);
    if (len <= 0) {
        mux.unlock();
        out.drop();
        return XData();

    }
    out.pts = indata.pts;
    mux.unlock();

//    LOGD("swr_convert success = %d ",len);
    return out;
}
