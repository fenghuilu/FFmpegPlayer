//
// Created by Chaofeng.Li on 2022/11/9.
//

#include "FFPlayerBuilder.h"
#include "FFDemux.h"
#include "FFDecode.h"
#include "FFResample.h"
#include "GLVideoView.h"
#include "SLAudioPlay.h"

IDemux *FFPlayerBuilder::createDemux() {
    IDemux *ff = new FFDemux();
    return ff;
}

IDecode *FFPlayerBuilder::createDecode() {
    IDecode *ff = new FFDecode();
    return ff;
}

IResample *FFPlayerBuilder::createResample() {
    IResample *ff = new FFResample();
    return ff;
}

IVideoView *FFPlayerBuilder::createVideoView() {
    IVideoView *ff = new GLVideoView();
    return ff;
}

IAudioPlay *FFPlayerBuilder::createAudioPlay() {
    IAudioPlay *ff = new SLAudioPlay();
    return ff;
}

IPlayer *FFPlayerBuilder::createPlayer(unsigned char index) {
    return IPlayer::get(index);
}

void FFPlayerBuilder::initHard(void *vm) {
    FFDecode::initHard(vm);
}
