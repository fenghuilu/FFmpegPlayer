//
// Created by Chaofeng.Li on 2022/11/9.
//

#ifndef FFMPEGPLAYER_FFPLAYERBUILDER_H
#define FFMPEGPLAYER_FFPLAYERBUILDER_H

#include "IPlayerBuilder.h"

class FFPlayerBuilder : public IPlayerBuilder {
public:
    static FFPlayerBuilder *get() {
        static FFPlayerBuilder builder;
        return &builder;
    }

    static void initHard(void *vm);

protected:
    virtual IDemux *createDemux();

    virtual IDecode *createDecode();

    virtual IResample *createResample();

    virtual IVideoView *createVideoView();

    virtual IAudioPlay *createAudioPlay();

    virtual IPlayer *createPlayer(unsigned char index = 0);

};


#endif //FFMPEGPLAYER_FFPLAYERBUILDER_H
