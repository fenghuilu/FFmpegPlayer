//
// Created by Chaofeng.Li on 2022/11/9.
//

#include "IPlayerBuilder.h"
#include "IPlayer.h"
#include "IDemux.h"
#include "IDecode.h"
#include "IResample.h"
#include "IVideoView.h"
#include "IAudioPlay.h"

IPlayer *IPlayerBuilder::buildPlayer(unsigned char index) {
    IPlayer *player = createPlayer(index);
    IDemux *demux = createDemux();
    IDecode *vdecode = createDecode();
    IDecode *adecode = createDecode();
    IResample *resample = createResample();
    IVideoView *view = createVideoView();
    IAudioPlay *audioPlay = createAudioPlay();

    demux->addObs(vdecode);
    demux->addObs(adecode);
    vdecode->addObs(view);
    adecode->addObs(resample);
    resample->addObs(audioPlay);


    player->demux = demux;
    player->adecode = adecode;
    player->vdecode = vdecode;
    player->resample = resample;
    player->audioPlayer = audioPlay;
    player->videoView = view;
    return player;
}
