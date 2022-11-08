//
// Created by chaofeng.li on 2022/1/29.
//

#ifndef FFMPEGPLAYER_XPARAMETER_H
#define FFMPEGPLAYER_XPARAMETER_H

struct AVCodecParameters;
class XParameter {
public:
    AVCodecParameters *para = 0;
    int channels = 2;
    int sample_rate = 44100;

};


#endif //FFMPEGPLAYER_XPARAMETER_H
