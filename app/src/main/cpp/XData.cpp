//
// Created by chaofeng.li on 2022/1/11.
//

#include "XData.h"

extern "C" {
#include <libavformat/avformat.h>
}

bool XData::alloc(int size, const char *d) {
    drop();
    type = UCHAR_TYPE;
    if (size <= 0)return false;
    this->data = new unsigned char[size];
    if (!this->data)return false;
    if (d) {
        memcpy(this->data, d, size);
    }
    this->size = size;
    return true;
}

void XData::drop() {
    if (!data)return;
    if (type == AVPACKET_TYPE)
        av_packet_free((AVPacket **) &data);
    else
        delete data;
    data = 0;
    size = 0;
}
