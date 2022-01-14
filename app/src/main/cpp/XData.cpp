//
// Created by chaofeng.li on 2022/1/11.
//

#include "XData.h"

extern "C" {
#include <libavformat/avformat.h>
}

void XData::drop() {
    if (!data)return;
    av_packet_free((AVPacket **) &data);
    data = 0;
    size = 0;
}
