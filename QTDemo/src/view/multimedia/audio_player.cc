/*
 *
 * audio_player.cc
 *
 * Created by Ruibin.Chow on 2023/05/22.
 * Copyright (c) 2023年 Ruibin.Chow All rights reserved.
 */

#include "audio_player.h"
#include <iostream>
#include "codec/lame_codec.h"
#include "foundation/assets.h"

void AudioPlayer::Init() {
    std::cout << "AudioPlayer::Init." << std::endl;
    LameCodec decoder;
    decoder.Init();
    
    using namespace Foundation;
    decoder.OpenDecode(Assets::AudioDirPath("canon.mp3"));
    
    static const int DECODE_BUFFER_SIZE = 4096;
    short pcm_l[DECODE_BUFFER_SIZE];
    short pcm_r[DECODE_BUFFER_SIZE];
    int nout = decoder.Decode(pcm_l, pcm_r);
    std::cout << "play:" << nout << std::endl;
//    nout = decoder.Decode(pcm_l, pcm_r);
//    std::cout << "play:" << nout << std::endl;
//    nout = decoder.Decode(pcm_l, pcm_r);
//    std::cout << "play:" << nout << std::endl;
//    nout = decoder.Decode(pcm_l, pcm_r);
//    std::cout << "play:" << nout << std::endl;
//    nout = decoder.Decode(pcm_l, pcm_r);
//    std::cout << "play:" << nout << std::endl;
    
    decoder.CloseDecode();
}
