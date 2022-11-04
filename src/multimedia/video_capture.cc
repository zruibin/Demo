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
    if (frameCallBack_) {
        frameCallBack_(frame);
    }
}


