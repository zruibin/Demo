/*
 * video_capture.h
 *
 * Created by Ruibin.Chow on 2022/11/03.
 * Copyright (c) 2022年 Ruibin.Chow All rights reserved.
 */

#ifndef VIDEO_CAPTURE_H
#define VIDEO_CAPTURE_H

#include <QCamera>
#include <QMediaCaptureSession>
#include <QMediaDevices>
#include <QVideoSink>
#include <QScopedPointer>


class VideoCaputer : public QObject {
    Q_OBJECT
public:
    VideoCaputer() = default;
    virtual ~VideoCaputer();

    void Init();
    void Start();
    void Stop();
    bool IsRunning() const { return running_; };
    bool IsInit() const { return initialize_; };

private slots:
    void OnFrameChanged(const QVideoFrame &frame);

private:
    QMediaCaptureSession captureSession_;
    QScopedPointer<QCamera> camera_;
    QVideoSink videoSink_;
    int frameNum_ = 0;
    bool running_ = false;
    bool initialize_ = false;
};


#endif /* !VIDEO_CAPTURE_H */
