/*
 *
 * video_capture.cc
 *
 * Created by Ruibin.Chow on 2022/11/03.
 * Copyright (c) 2022年 Ruibin.Chow All rights reserved.
 */

#include "video_capture.h"

VideoCaputer::~VideoCaputer() {
    if (running_) {
        camera_->stop();
        running_ = false;
    }
}

void VideoCaputer::Init() {
    qDebug()<< "VideoCaputer::Init.";
    QCameraDevice camerDevice = QMediaDevices::defaultVideoInput();
    camera_.reset(new QCamera(camerDevice));

    connect(camera_.get(), &QCamera::errorOccurred, this, [this](){
        qDebug()<< "camera error occurred"<<this->camera_->errorString();
    });

    captureSession_.setCamera(camera_.data());
    captureSession_.setVideoSink(&videoSink_);

    connect(&videoSink_, &QVideoSink::videoFrameChanged, this, &VideoCaputer::OnFrameChanged);
    initialize_ = true;
}

void VideoCaputer::Start() {
    qDebug()<< "VideoCaputer::Start.";
    camera_->start();
    running_ = true;
}

void VideoCaputer::Stop() {
    qDebug()<< "VideoCaputer::Stop.";
    camera_->stop();
    running_ = false;
}

void VideoCaputer::OnFrameChanged(const QVideoFrame &frame) {
    QVideoFrame videoFrame = frame;
    /**
     * 我的电脑上返回的格式是Format_NV12：18，每个像素1.5字节，一帧1382400字节，
     * 如果你需要用别的YUV或者RGB格式，则需要自己设置QVideoSink或者QCamera格式
     */
    QVideoFrameFormat::PixelFormat pixelFormat = videoFrame.pixelFormat();
    int width = videoFrame.width();
    int height = videoFrame.height();
    int planeCount = videoFrame.planeCount();
    uchar *pdata = nullptr;
    int len = 0;

    videoFrame.map(QVideoFrame::ReadOnly);

    for (int i = 0; i < planeCount; i++) {
        pdata = videoFrame.bits(i);
        len = videoFrame.mappedBytes(i);
//        tmpfile.write((const char *)pdata, len);
    }

    ++frameNum_;
    qDebug("%d, %d, %d, %d, %d, %p, %d", frameNum_,
           pixelFormat, width, height, planeCount, pdata, len);

    videoFrame.unmap();
}


