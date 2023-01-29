/*
 *
 * app.cc
 *
 * Created by Ruibin.Chow on 2022/10/20.
 * Copyright (c) 2022年 Ruibin.Chow All rights reserved.
 */

#include "app.h"
#include <filesystem>
#include "log/logging.h"
#include "util/timer.h"
#include "util/util.h"
#include "platform/platform.h"
#include "foundation/assets.h"

#if VIEW_DISPLAY
#include <QApplication>
#include <QTextCodec>
#include <QDir>
#include "view/window.h"
#endif


extern "C" {
#include <libavutil/ffversion.h>
#include <libavformat/avformat.h>
}


namespace app {

/**
* @brief 将一个AVRational类型的分数转换为double类型的浮点数
* @param r:r为一个AVRational类型的结构体变量，成员num表示分子，成员den表示分母，r的值即为(double)r.num / (double)r.den。用这种方法表示可以最大程度地避免精度的损失
* @return 如果变量r的分母den为0，则返回0（为了避免除数为0导致程序死掉）;其余情况返回(double)r.num / (double)r.den
*/
static double r2d(AVRational r) {
    return r.den == 0 ? 0 : (double)r.num / (double)r.den;
}

void testFFMpeg() {
    using namespace Foundation;
    Log(INFO) << "Project Dir: " << Assets::ProjectDirPath();
    Log(INFO) << "FFMpeg Version: " << FFMPEG_VERSION;
    
    std::string moviePath{Assets::AssetsDirPath().append("fui.mp4")};
    Log(INFO) << "Movie Dir: " << moviePath;
    const char *path = moviePath.c_str();
    //const char *path = "audio1.mp3";
    
//    av_register_all(); //初始化所有组件，只有调用了该函数，才能使用复用器和编解码器。否则，调用函数avformat_open_input会失败，无法获取媒体文件的信息
    avformat_network_init(); //打开网络流。这里如果只需要读取本地媒体文件，不需要用到网络功能，可以不用加上这一句
    AVDictionary *opts = NULL;
    AVFormatContext *ic = NULL; //AVFormatContext是描述一个媒体文件或媒体流的构成和基本信息的结构体
    int re = avformat_open_input(&ic, path, NULL, &opts); //媒体打开函数，调用该函数可以获得路径为path的媒体文件的信息，并把这些信息保存到指针ic指向的空间中（调用该函数后会分配一个空间，让指针ic指向该空间）
    
    if (re != 0) {
        //如果打开媒体文件失败，打印失败原因。比如，如果上面没有调用函数av_register_all，则会打印“XXX failed!:Invaliddata found when processing input”
        char buf[1024] = { 0 };
        av_strerror(re, buf, sizeof(buf) - 1);
        printf("open %s failed!:%s", path, buf);
    } else {//打开媒体文件成功
        printf("打开媒体文件 %s 成功!\n", path);
        avformat_find_stream_info(ic, NULL); //调用该函数可以进一步读取一部分视音频数据并且获得一些相关的信息。调用avformat_open_input之后，我们无法获取到正确和所有的媒体参数，所以还得要调用avformat_find_stream_info进一步的去获取。
//        printf("媒体文件名称:%s\n", ic->name); //调用avformat_open_input读取到的媒体文件的路径/名字
        printf("视音频流的个数:%d\n", ic->nb_streams);//视音频流的个数，如果一个媒体文件既有音频，又有视频，则nb_streams的值为2。如果媒体文件只有音频，则值为1
        //printf("duration:%d\n", ic->duration);
        printf("媒体文件的平均码率:%lldbps\n", ic->bit_rate);    //媒体文件的平均码率,单位为bps
        int tns, thh, tmm, tss;
        tns = (ic->duration) / AV_TIME_BASE;
        thh = tns / 3600;
        tmm = (tns % 3600) / 60;
        tss = (tns % 60);
        printf("媒体文件总时长：%d时%d分%d秒\n", thh, tmm, tss); //通过上述运算，可以得到媒体文件的总时长

        printf("\n");
        for (int i = 0; i < ic->nb_streams; i++) //通过遍历的方式读取媒体文件视频和音频的信息，新版本的FFmpeg新增加了函数av_find_best_stream，也可以取得同样的效果，但这里为了兼容旧版本还是用这种遍历的方式
        {
            AVStream *as = ic->streams[i];
            if (AVMEDIA_TYPE_AUDIO == as->codecpar->codec_type) //如果是音频流，则打印音频的信息
            {
                printf("音频信息:\n");
                printf("index:%d\n", as->index);  //如果一个媒体文件既有音频，又有视频，则音频index的值一般为1。但该值不一定准确，所以还是得通过as->codecpar->codec_type判断是视频还是音频
                printf("音频采样率:%dHz\n", as->codecpar->sample_rate); //音频编解码器的采样率，单位为Hz
                if (AV_SAMPLE_FMT_FLTP == as->codecpar->format)   //音频采样格式
                {
                    printf("音频采样格式:AV_SAMPLE_FMT_FLTP\n");
                }
                else if (AV_SAMPLE_FMT_S16P == as->codecpar->format)
                {
                    printf("音频采样格式:AV_SAMPLE_FMT_S16P\n");
                }
                printf("音频信道数目:%d\n", as->codecpar->channels); //音频信道数目
                if (AV_CODEC_ID_AAC == as->codecpar->codec_id)   //音频压缩编码格式
                {
                    printf("音频压缩编码格式:AAC\n");
                }
                else if (AV_CODEC_ID_MP3 == as->codecpar->codec_id)
                {
                    printf("音频压缩编码格式:MP3\n");
                }
                int DurationAudio = (as->duration) * r2d(as->time_base); //音频总时长，单位为秒。注意如果把单位放大为毫秒或者微妙，音频总时长跟视频总时长不一定相等的
                printf("音频总时长：%d时%d分%d秒\n", DurationAudio / 3600, (DurationAudio % 3600) / 60, (DurationAudio % 60)); //将音频总时长转换为时分秒的格式打印到控制台上
                printf("\n");
            }
            else if (AVMEDIA_TYPE_VIDEO == as->codecpar->codec_type)  //如果是视频流，则打印视频的信息
            {
                printf("视频信息:\n");
                printf("index:%d\n", as->index);   //如果一个媒体文件既有音频，又有视频，则视频index的值一般为0。但该值不一定准确，所以还是得通过as->codecpar->codec_type判断是视频还是音频
                printf("视频帧率:%lffps\n", r2d(as->avg_frame_rate)); //视频帧率,单位为fps，表示每秒出现多少帧
                if (AV_CODEC_ID_MPEG4 == as->codecpar->codec_id) //视频压缩编码格式
                {
                    printf("视频压缩编码格式:MPEG4\n");
                }
                printf("帧宽度:%d 帧高度:%d\n", as->codecpar->width, as->codecpar->height); //视频帧宽度和帧高度
                int DurationVideo = (as->duration) * r2d(as->time_base); //视频总时长，单位为秒。注意如果把单位放大为毫秒或者微妙，音频总时长跟视频总时长不一定相等的
                printf("视频总时长：%d时%d分%d秒\n", DurationVideo / 3600, (DurationVideo % 3600) / 60, (DurationVideo % 60)); //将视频总时长转换为时分秒的格式打印到控制台上
                printf("\n");
            }
        }

        //av_dump_format(ic, 0, path, 0);
    }
    
    if (ic) {
        avformat_close_input(&ic); //关闭一个AVFormatContext,和函数avformat_open_input()成对使用
    }
}

void App::Init() {
    logger::SetMinWriteLogLevel(logger::VERBOSE);
    platform::thread_set_name("FFMpegDemo.main-thread");
    Log(INFO) << "App Init Start.";
    Log(VERBOSE) << "Current Thread Name: " << platform::thread_get_current_name();
    
    Log(INFO) << "App Init End.";
}

void App::Run(int &argc, char **argv) {
    using namespace Foundation;
#if VIEW_DISPLAY
    QApplication::setAttribute(Qt::AA_UseOpenGLES);
    QApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
    
    QApplication app(argc, argv);
    app.setApplicationName("FFMpegDemo");
    app.setApplicationVersion("V1.0.0");
    
    QString dirPath = QCoreApplication::applicationDirPath().append("/Data");
    QDir dir(dirPath);
    if(!dir.exists()) dir.mkdir(dirPath);
    logger::SetDefaultLoggerDir(dirPath.toStdString());
    
    QString assetsPath = QCoreApplication::applicationDirPath().append("/../Resources/assets/");
    Assets::SetAssetsDirPath(assetsPath.toStdString());
#else
    namespace fs = std::filesystem;
    fs::path dst = fs::current_path() / std::string("./Data");
    if (!fs::exists(dst)) fs::create_directory(dst);
    logger::SetDefaultLoggerDir(dst.string());
    
    Assets::SetAssetsDirPath(Assets::ProjectDirPath().append("/assets/"));
#endif
    
    Init();
    testFFMpeg();
    
#if VIEW_DISPLAY
    view::Window w;
    w.show();
    
    app.exec();
#endif
}

}


