/*
 * lame_codec.h
 *
 * Created by Ruibin.Chow on 2023/05/22.
 * Copyright (c) 2023年 Ruibin.Chow All rights reserved.
 */

#ifndef LAME_CODEC_H
#define LAME_CODEC_H

#include <string>
#include <functional>
#include <lame/lame.h>

class LameCodec {
    
public:
    explicit LameCodec();
    virtual ~LameCodec();
    
    void Init();
    
    bool OpenDecode(const std::string&);
    bool CloseDecode();
    
    /**
     nout:  -2    : reach file end
            -1    : decoding error
             0    : need more data before we can complete the decode
            >0    : returned 'nout' samples worth of data in pcm_l,pcm_r
     */
    int Decode(short*, short*);
    
private:
    int nChannels_{-1};
    int sampleRate_{-1};
    int bitrate_{-1};
    int frameSize_{-1};
    long fileSize_{-1};
    long fileCurrent_{-1};
    
    FILE *decoderFile_{nullptr};
    lame_global_flags* decoderLame_{nullptr};
    hip_t decoderHip_{nullptr};
};


#endif /* !LAME_CODEC_H */
