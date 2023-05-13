/*
 * audio_capture.h
 *
 * Created by Ruibin.Chow on 2023/05/13.
 * Copyright (c) 2023年 Ruibin.Chow All rights reserved.
 */

#ifndef AUDIO_CAPTURE_H
#define AUDIO_CAPTURE_H

#include <functional>
#include <QAudioInput>
#include <QMediaCaptureSession>
#include <QMediaDevices>
#include <QAudioSink>
#include <QMediaRecorder>
#include <QScopedPointer>

class AudioCaputer : public QObject {
    Q_OBJECT
public:
    explicit AudioCaputer();
    virtual ~AudioCaputer();

    void Init();
    void Start();
    void Stop();
    bool IsRunning() const { return running_; };
    bool IsInit() const { return initialize_; };

private:
    QMediaCaptureSession captureSession_;
    QScopedPointer<QAudioInput> audioInput_;
    QScopedPointer<QMediaRecorder> audioRecorder_;
    
    bool running_ = false;
    bool initialize_ = false;
};

#endif /* !AUDIO_CAPTURE_H */
