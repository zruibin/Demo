/*
 *
 * audio_capture.cc
 *
 * Created by Ruibin.Chow on 2023/05/13.
 * Copyright (c) 2023年 Ruibin.Chow All rights reserved.
 */

#include "audio_capture.h"
#include <QDebug>
#include <QMediaFormat>
#include <QUrl>
#include <QFileDialog>
#include "permission.h"

/*
 https://blog.csdn.net/qq582880551/article/details/123764531
 */

AudioCaputer::AudioCaputer() {
    Qt::PermissionStatus status = Permission::GetInstance()->CheckMicrophonePermission();
    if (status != Qt::PermissionStatus::Granted) {
        Permission::GetInstance()->RequestMicrophonePermission([this](const QPermission &permission) {
            qDebug() << "MicrophonePermission status: " << permission.status();
        });
    }
}

AudioCaputer::~AudioCaputer() {
    if (running_) {
        Stop();
        running_ = false;
    }
}

void AudioCaputer::Init() {
    qDebug() << "AudioCaputer::Init.";
    
    QAudioDevice audioDevice;
    const QList<QAudioDevice> availableAudios = QMediaDevices::audioInputs();
    for (const QAudioDevice &availableAudio : availableAudios) {
        if (availableAudio == QMediaDevices::defaultAudioInput())
            audioDevice = availableAudio;
    }
    audioInput_.reset(new QAudioInput());
    audioInput_->setDevice(audioDevice);
    
    audioRecorder_.reset(new QMediaRecorder);
    captureSession_.setRecorder(audioRecorder_.data());
    captureSession_.setAudioInput(audioInput_.data());
    
    QMediaFormat format;
    format.setAudioCodec(QMediaFormat::AudioCodec::MP3);
    format.setFileFormat(QMediaFormat::MP3);
    audioRecorder_->setMediaFormat(format);

    audioRecorder_->setAudioSampleRate(44100); //采样频率。8kHz(电话)、44.1kHz(CD)、48kHz(DVD)。
    audioRecorder_->setAudioChannelCount(1); //通道个数。常见的音频有立体声(stereo)和单声道(mono)两种类型，立体声包含左声道和右声道。另外还有环绕立体声等其它不太常用的类型。
    audioRecorder_->setAudioBitRate(64000); // 32000, 64000, 96000, 128000
    
    initialize_ = true;
}

void AudioCaputer::Start() {
    qDebug() << "AudioCaputer::Start.";
    QString fileName = QFileDialog::getSaveFileName();
    QUrl url = QUrl::fromLocalFile(fileName);
    qDebug() << "fileName: " << fileName;
    audioRecorder_->setOutputLocation(url);
    audioRecorder_->record();
    running_ = true;
}

void AudioCaputer::Stop() {
    qDebug() << "AudioCaputer::Stop.";
    audioRecorder_->stop();
    running_ = false;
}

