/*
 *
 * ffmpeg_test.cc
 *
 * Created by Ruibin.Chow on 2023/02/22.
 * Copyright (c) 2023年 Ruibin.Chow All rights reserved.
 */

#include <iostream>
#include <gtest/gtest.h>
#include <gmock/gmock.h>

extern "C" {
#include <libavutil/ffversion.h>
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libavcodec/codec.h>
#include <libavutil/opt.h>
}

/*
 https://ffmpeg.xianwaizhiyin.net/
 https://github.com/lokenetwork/FFmpeg-Principle
 https://ffmpeg.xianwaizhiyin.net/api-ffmpeg/av_demuxer_iterate.html
 https://ffmpeg.xianwaizhiyin.net/api-ffmpeg/av_opt_next.html
 
 https://www.cnblogs.com/leisure_chn/p/10040202.html
 */


// ffmpeg库版本
TEST(FFMpegTest, VersionTest) {
    std::cout << "FFMpeg Version: " << FFMPEG_VERSION << std::endl;
}

// 解封装
TEST(FFMpegTest, DemuxerTest) {
    void* ifmt_opaque = NULL;
    const AVInputFormat *ifmt  = NULL;
    while ((ifmt = av_demuxer_iterate(&ifmt_opaque))) {
        std::cout << "demuxer name is " << ifmt->name << std::endl;
    }
}

// 封装
TEST(FFMpegTest, MuxerTest) {
    void* ofmt_opaque = NULL;
    const AVOutputFormat *ofmt  = NULL;
    while ((ofmt = av_muxer_iterate(&ofmt_opaque))) {
        std::cout << "muxer name is " << ofmt->name << std::endl;
    }
}

// 遍历编解器的公共参数跟私有参数
TEST(FFMpegTest, CodecOptTest) {
    const AVCodec *encode = avcodec_find_encoder(AV_CODEC_ID_HEVC);
    AVCodecContext* enc_ctx = avcodec_alloc_context3(encode);
    const AVOption *opt = NULL;
    while ((opt = av_opt_next(enc_ctx, opt))) {
        std::cout << "common opt_name is %s " << opt->name << std::endl;
    }
    while ((opt = av_opt_next(enc_ctx->priv_data, opt))) {
        std::cout << "private opt_name is %s " << opt->name << std::endl;
    }
    avcodec_free_context(&enc_ctx);
}
