/*
 *
 * audio_capture.cc
 *
 * Created by Ruibin.Chow on 2023/05/13.
 * Copyright (c) 2023年 Ruibin.Chow All rights reserved.
 */

#include "audio_capture.h"
#include <QDebug>
#include <QUrl>
#include <QFileDialog>
#include "permission.h"
#include <QDir>
#include <QStandardPaths>

/*
 https://blog.csdn.net/qq582880551/article/details/123764531
 */

AudioCaputer::AudioCaputer() {
    qDebug() << "homePath:" << QDir::homePath();
    QString desktop = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);
    qDebug() << "Desktop Path:" << desktop;
    filePath_.reset(new QString(desktop.append("/record.pcm")));
    
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
    audioProbe_->stop();
}

void AudioCaputer::Init() {
    qDebug() << "AudioCaputer::Init.";
    
    QAudioDevice audioDevice;
    const QList<QAudioDevice> availableAudios = QMediaDevices::audioInputs();
    for (const QAudioDevice &availableAudio : availableAudios) {
        if (availableAudio == QMediaDevices::defaultAudioInput())
            audioDevice = availableAudio;
    }
    
    QAudioFormat format;
    format.setSampleRate(44100);
    format.setSampleFormat(QAudioFormat::Int16);
    format.setChannelCount(2);
    audioSource_.reset(new QAudioSource(audioDevice, format));
    audioProbe_.reset(new AudioProbeDevice(this));
    audioProbe_->start(audioSource_.data());
    connect(audioProbe_.get(), &AudioProbeDevice::OnBufferChanged, this, [this](const QAudioBuffer &buffer) {
        QAudioFormat format = buffer.format();
        qDebug("sampleRate:%d, channelCount:%d, sampleFormat:%d. "
               "SampleCount:%lld, FrameCount:%lld, ByteCount:%lld",
               format.sampleRate(), format.channelCount(), format.sampleFormat(),
               buffer.sampleCount(), buffer.frameCount(), buffer.byteCount());
        //ffplay -ar 44100 -ac 2 -f s16le -i record.pcm
        file_->write(buffer.constData<char>(), buffer.byteCount());
    });
    connect(audioProbe_.get(), &AudioProbeDevice::OnLevelChanged, this, [](const float level) {
        qDebug() << "OnLevelChanged:" << level;
    });
    
//    audioInput_.reset(new QAudioInput());
//    audioInput_->setDevice(audioDevice);
//    audioRecorder_.reset(new QMediaRecorder);
//    captureSession_.setRecorder(audioRecorder_.data());
//    captureSession_.setAudioInput(audioInput_.data());
//
//    QMediaFormat format;
//    format.setAudioCodec(QMediaFormat::AudioCodec::MP3);
//    format.setFileFormat(QMediaFormat::MP3);
//    audioRecorder_->setMediaFormat(format);
//
//    audioRecorder_->setAudioSampleRate(44100); //采样频率。8kHz(电话)、44.1kHz(CD)、48kHz(DVD)。
//    audioRecorder_->setAudioChannelCount(1); //通道个数。常见的音频有立体声(stereo)和单声道(mono)两种类型，立体声包含左声道和右声道。另外还有环绕立体声等其它不太常用的类型。
//    audioRecorder_->setAudioBitRate(64000); // 32000, 64000, 96000, 128000
    
    initialize_ = true;
}

void AudioCaputer::Start() {
    qDebug() << "AudioCaputer::Start.";
//    QString fileName = QFileDialog::getSaveFileName();
//    QUrl url = QUrl::fromLocalFile(fileName);
//    qDebug() << "fileName: " << fileName;
//    audioRecorder_->setOutputLocation(url);
//    audioRecorder_->record();
    
    file_.reset(new QFile(*filePath_.get()));
    if (!file_->open(QIODevice::WriteOnly)) {
        qDebug() << "写文件失败！";
    }
    
    auto *io = audioSource_->start();
    if (!io) {
        qDebug() << "AudioCaputer::Start Error! " << io;
        return;
    }
    
    connect(io, &QIODevice::readyRead, [this, io]() {
        static const qint64 BufferSize = 4096;
        const qint64 len = qMin(audioSource_->bytesAvailable(), BufferSize);
        QByteArray buffer(len, 0);
        qint64 l = io->read(buffer.data(), len);
        if (l > 0) {
            audioProbe_->writeData(buffer.data(), l);
        }
    });
    
    running_ = true;
}

void AudioCaputer::Stop() {
    qDebug() << "AudioCaputer::Stop.";
//    audioRecorder_->stop();
    audioSource_->stop();
    file_->close();
    running_ = false;
}

