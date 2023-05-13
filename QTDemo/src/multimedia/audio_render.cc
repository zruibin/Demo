/*
 *
 * audio_render.cc
 *
 * Created by Ruibin.Chow on 2023/05/13.
 * Copyright (c) 2023年 Ruibin.Chow All rights reserved.
 */

#include "audio_render.h"
#include <QMediaDevices>
#include <QDebug>
#include <QDir>
#include <QStandardPaths>
#include <QThread>
#include <thread>
#include <foundation/assets.h>


AudioRender::AudioRender() {
    filePath_.reset(new QString(Foundation::Assets::AudioDirPath("canon.pcm").c_str()));
}

AudioRender::~AudioRender() {
    if (running_) {
        Stop();
    }
}

void AudioRender::Init() {
    
    QAudioFormat format;
    format.setSampleRate(44100);
    format.setSampleFormat(QAudioFormat::Int16);
    format.setChannelCount(2);
    QAudioDevice audioDevice = QMediaDevices::defaultAudioOutput();
    audioSink_.reset(new QAudioSink(audioDevice, format));
    
    initialize_ = true;
}

void AudioRender::Start() {
    auto *io = audioSink_->start();
    if (!io) {
        qDebug() << "AudioRender::Start Error! " << io;
        return;
    }
    
    std::thread playThred([this, io](){
        file_ = fopen(filePath_->toStdString().c_str(), "rb");
        int size = 4096;
        char *buf = new char[size];
        
        while (!feof(file_)) {
            if (!running_) {
                break;
            }
            if (audioSink_->bytesFree() < size) {
                QThread::msleep(1);
                continue;
            }
            int len = fread(buf, 1, size, file_);
            if (len <= 0) break;
            io->write(buf, len);
        }
        fclose(file_);
        delete []buf;
        buf = 0;
    });
    playThred.detach();
    
    running_ = true;
}

void AudioRender::Stop() {
    audioSink_->stop();
    running_ = false;
}


