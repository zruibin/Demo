/*
 *
 * lame_codec.cc
 *
 * Created by Ruibin.Chow on 2023/05/22.
 * Copyright (c) 2023年 Ruibin.Chow All rights reserved.
 */

#include "lame_codec.h"
#include <iostream>
#include <stdio.h>
#include <vector>
#include <filesystem>
#include <iomanip>


/*
 https://www.jianshu.com/p/c9feaec59bab
 https://www.cnblogs.com/musicfans/archive/2012/07/16/2819299.html
 https://www.jianshu.com/p/fb531239cd79
 https://blog.csdn.net/geekster/article/details/7888099
 http://www.zzvips.com/article/104036.html
 */

struct mp3 {
    char header[3]; // 标签头是"ID3", 非必须
    char footer[3]; // 标签头是"TAG"，必须
    char title[30]; // 标题
    char artist[30]; // 作者
    char album[30]; // 专集
    char year[4]; // 出品年代
    char comment[30]; // 备注
};

static bool isMp3File(FILE *fp) {
    struct mp3 mp3_info; //存放读取的mp3 ID3V1信息
    fseek(fp, 0, SEEK_SET);
    
    //读取标签头
    fread(mp3_info.header, sizeof(char), 3, fp);
    // mp3以末尾的128字节存放，偏移-128
    fseek(fp, -128*sizeof(char), SEEK_END);
    fread(mp3_info.footer, sizeof(char), 3, fp);
    
    // 判断开头是不是ID3
    if(!(mp3_info.header[0] == 'I'
         && mp3_info.header[1] == 'D'
         && mp3_info.header[2] == '3')) {
        printf("file contains no ID3 tag\n");
        if(!(mp3_info.footer[0] == 'T'
             && mp3_info.footer[1] == 'A'
             && mp3_info.footer[2] == 'G')) {
            printf("file contains no TAG tag\n");
            return false;
        }
    }
    printf("header: %s\n", mp3_info.header);
    printf("footer: %s\n", mp3_info.footer);
    
//    //读取信息
    fread(mp3_info.title, sizeof(char), 30, fp);
    fread(mp3_info.artist, sizeof(char), 30, fp);
    fread(mp3_info.album, sizeof(char), 30, fp);
    fread(mp3_info.year, sizeof(char), 4, fp);
    fread(mp3_info.comment, sizeof(char), 30, fp);

//    //打印信息
    printf("title: %s\n", mp3_info.title);
    printf("artist: %s\n", mp3_info.artist);
    printf("blbum: %s\n", mp3_info.album);
    printf("year: %s\n", mp3_info.year);
    printf("comment: %s\n", mp3_info.comment);
    
    return true;
}

static void lameError(const char *format, va_list ap) {
    printf("lameError:");
    printf(format, ap);
}

static void lameDebug(const char *format, va_list ap) {
    printf("lameDebug:");
    printf(format, ap);
}

static void lameMsg(const char *format, va_list ap) {
    printf("lameMsg:");
    printf(format, ap);
}

LameCodec::LameCodec() {

}

LameCodec::~LameCodec() {

}

void LameCodec::Init() {
    std::cout << "Init." << std::endl;
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
    
    decoderFile_ = fopen(mp3Path.c_str(), "rb");
    
    if (!isMp3File(decoderFile_)) {
        std::cout << "OpenDecode->It was not mp3 file." << std::endl;
        return false;
    }
    
    decoderLame_ = lame_init();
    lame_set_decode_only(decoderLame_, 1);
    decoderHip_ = hip_decode_init();
    
    hip_set_errorf(decoderHip_, lameError);
    hip_set_debugf(decoderHip_, lameDebug);
    hip_set_msgf(decoderHip_, lameMsg);
    
    fseek(decoderFile_, 0, SEEK_SET);
    
    const int MP3_SIZE = 1024;
    unsigned char mp3_buffer[MP3_SIZE];
    mp3data_struct mp3data;
    
    short pcm_l[MP3_SIZE];
    short pcm_r[MP3_SIZE];
    
    int read;
    while ((read = fread(mp3_buffer, sizeof(char), MP3_SIZE, decoderFile_)) > 0) {
        int nout = hip_decode1_headers(decoderHip_, mp3_buffer, read, pcm_l, pcm_r, &mp3data);
        if (nout < 0) {
            std::cout << "OpenDecode->Error:" << nout << std::endl;
            return false;
        }
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
    fileSize_ = ftell(decoderFile_);
    
    // 一帧时长 = 数据长度 / 采样率
    double frameDuration = frameSize_ * 1.0f / sampleRate_;
    // 总播放时间 = 文件大小 * 8 / 比特率
    double allDuration = (1.0f * fileSize_ * 8) / (bitrate_ * 1000);
    
    std::cout << std::fixed << std::setprecision(3)
            << "OpenDecode->文件大小:" << fileSize_ << "b = "
            << (1.0f * fileSize_ / 1024 / 1024) << "M"
            << " 通道数:" << nChannels_
            << " 采样率:" << sampleRate_
            << "Hz 比特率:" << bitrate_
            << "kbps 一帧大小:" << frameSize_
            << "b 一帧时长:" << frameDuration
            << "s 总时长:" << allDuration << "s"
            << std::endl;
//    std::cout << "OpenDecode->nsamp: " << mp3data.nsamp
//            << " totalframes:" << mp3data.totalframes
//            << " framenum:" << mp3data.framenum
//            << std::endl;

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
    
    const int DECODE_BUFFER_SIZE = 256;

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
        fileCurrent_ += DECODE_BUFFER_SIZE;
        if (nout > 0) {
            // 达到一帧，时长 = 数据长度 / 采样率
            duration += nout * 1.0f / sampleRate_;
            return nout;
        }
    }
    std::cout << "Decode->duration:" << duration << "s precent: "
            << std::fixed << std::setprecision(2)
            << (fileCurrent_ * 1.0f / fileSize_) * 100
            << "% current:" << fileCurrent_ << "b" << std::endl;
    
    return -2; // reach file end
}
