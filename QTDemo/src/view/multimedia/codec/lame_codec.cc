/*
 *
 * lame_codec.cc
 *
 * Created by Ruibin.Chow on 2023/05/22.
 * Copyright (c) 2023年 Ruibin.Chow All rights reserved.
 */

#include "lame_codec.h"
#include <iostream>
#include <vector>
#include <filesystem>


/*
 https://www.jianshu.com/p/c9feaec59bab
 https://www.cnblogs.com/musicfans/archive/2012/07/16/2819299.html
 https://www.jianshu.com/p/fb531239cd79
 */

LameCodec::LameCodec() {

}

LameCodec::~LameCodec() {

}

void LameCodec::Init() {
    std::cout << "Init." << std::endl;
    hip_t hip = hip_decode_init();
    if (!hip) {
        std::cout << "创建mp3 codec失败" << std::endl;
        return;
    }
}


bool LameCodec::OpenDecode(const std::string& mp3Path) {
    std::cout << "OpenDecode." << std::endl;
    if (mp3Path.length() == 0) {
        std::cout << "OpenDecode->data path was nil." << std::endl;
        return false;
    }
    namespace fs = std::filesystem;
    fs::path dst(mp3Path);
    if (!fs::exists(dst)) {
        std::cout << "OpenDecode->file not exists." << std::endl;
        return false;
    }
    
    decoderLame_ = lame_init();
    lame_set_decode_only(decoderLame_, 1);
    decoderHip_ = hip_decode_init();
    
    decoderFile_ = fopen(mp3Path.c_str(), "rb");
    fseek(decoderFile_, 0, SEEK_SET);
    
    const int MP3_SIZE = 1024;
    unsigned char mp3_buffer[MP3_SIZE];
    mp3data_struct mp3data;
    
    int read;
    while ((read = fread(mp3_buffer, sizeof(char), MP3_SIZE, decoderFile_)) > 0) {
        hip_decode1_headers(decoderHip_, mp3_buffer, read, NULL, NULL, &mp3data);
        if (mp3data.header_parsed == 1) {
            /*
             Metadata:
                 title           : Canon in D Major (Pachebel)
                 artist          : Lee Galloway
                 date            : 1999
                 comment         : http://mp3.com/LeeGalloway
               Duration: 00:02:51.08, start: 0.000000, bitrate: 128 kb/s
               Stream #0:0: Audio: mp3, 44100 Hz, stereo, fltp, 128 kb/s
             */
            nChannels_ = mp3data.stereo;
            sampleRate_ = mp3data.samplerate;
            bitrate_ = mp3data.bitrate;
            frameSize_ = mp3data.framesize;
            break;
        }
    }
    
    fseek(decoderFile_, 0L, SEEK_END);
    size_ = ftell(decoderFile_);
    
    int bufferLen = sampleRate_ * nChannels_ * 2 / 50;
    long sizeOfSecond = mp3data.framesize / 8 * mp3data.bitrate * 1000 / mp3data.samplerate;
    
    //2737342b -> 2,737,342 字节（磁盘上的2.7 MB）
    std::cout << "OpenDecode->fileSize: " << size_
            << " Channels:" << nChannels_
            << " Samplerate:" << sampleRate_
            << " Bitrate:" << bitrate_
            << " FrameSize:" << frameSize_
            << " sizeOfSecond:" << sizeOfSecond
            << std::endl;
    std::cout << "OpenDecode->nsamp: " << mp3data.nsamp
            << " totalframes:" << mp3data.totalframes
            << " framenum:" << mp3data.framenum
            << std::endl;

    fseek(decoderFile_ ,0, SEEK_SET); //将文件指针拨回起点
    return true;
}

bool LameCodec::CloseDecode() {
    std::cout << "CloseDecode." << std::endl;
    if (decoderHip_ != nullptr) {
        hip_decode_exit(decoderHip_);
        decoderHip_ = nullptr;
    }
    if (decoderLame_ != nullptr) {
        lame_close(decoderLame_);
        decoderLame_ = nullptr;
    }
    if (decoderFile_ != nullptr) {
        fclose(decoderFile_);
        decoderFile_ = nullptr;
    }
}

int LameCodec::Decode(short* pcmL, short* pcmR) {
    int nout = -1;
    
    static const int DECODE_BUFFER_SIZE = 256;
    
//    short pcm_l[DECODE_BUFFER_SIZE];
//    short pcm_r[DECODE_BUFFER_SIZE];
    int bytes;
    unsigned char mp3_buf[DECODE_BUFFER_SIZE];

    static double duration = 0;
    while ((bytes = fread(mp3_buf, DECODE_BUFFER_SIZE, 1, decoderFile_)) > 0) {
        nout = hip_decode1(decoderHip_,
                           mp3_buf,
                           256,
                           pcmL, pcmR);
        if (nout < 0) {
            std::cout << "Decode->Error:" << nout << std::endl;
            return nout;
        }
        current_ += DECODE_BUFFER_SIZE;
        if (nout > 0) {
            // 达到一帧，时长 = 数据长度 / 采样率
            duration += nout * 1.0f / sampleRate_;
            std::cout << "Decode->duration:" << duration << std::endl;
            std::cout << "Decode->current:" << current_ << std::endl;
//            return nout;
        }
    }
    
    return -2; // reach file end
}
