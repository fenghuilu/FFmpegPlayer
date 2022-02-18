//
// Created by chaofeng.li on 2022/1/11.
//

#ifndef FFMPEGPLAYER_XDATA_H
#define FFMPEGPLAYER_XDATA_H


struct XData {
    unsigned char *data = 0;
    int size = 0;
    bool isAudio = false;

    void drop();

};


#endif //FFMPEGPLAYER_XDATA_H
