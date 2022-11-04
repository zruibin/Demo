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

    if (camera_->cameraFormat().isNull()) {
        auto formats = camerDevice.videoFormats();
        if (!formats.isEmpty()) {
            // Choose a decent camera format: Maximum resolution at at least 30 FPS
            // we use 29 FPS to compare against as some cameras report 29.97 FPS...
            QCameraFormat bestFormat;
            for (const auto &fmt : formats) {
//                if (bestFormat.maxFrameRate() < 29
//                    && fmt.maxFrameRate() > bestFormat.maxFrameRate())
//                    bestFormat = fmt;
//                else if (bestFormat.maxFrameRate() == fmt.maxFrameRate()
//                            && bestFormat.resolution().width() * bestFormat.resolution().height()
//                                    < fmt.resolution().width() * fmt.resolution().height())
//                    bestFormat = fmt;
                qDebug()<< "formats->pixelFormat: "
                        << QVideoFrameFormat::pixelFormatToString(fmt.pixelFormat())
                        << " minFrameRate:" << fmt.minFrameRate()
                        << " maxFrameRate:" << fmt.maxFrameRate()
                        << "resolution:"<<fmt.resolution().width()<<"x"<<fmt.resolution().height();

                if (fmt.pixelFormat() == QVideoFrameFormat::Format_NV12) {
                    bestFormat = fmt;
                }
            }

            camera_->setCameraFormat(bestFormat);
            qDebug()<< "BestFormat->pixelFormat: "
                    << QVideoFrameFormat::pixelFormatToString(bestFormat.pixelFormat())
                    << " minFrameRate:" << bestFormat.minFrameRate()
                    << " maxFrameRate:" << bestFormat.maxFrameRate()
                    << "resolution:"<<bestFormat.resolution().width()<<"x"<<bestFormat.resolution().height();
        }
    }

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
        QVideoFrame newFrame(frame);
//        newFrame.setRotationAngle(QVideoFrame::Rotation270);
        newFrame.setMirrored(mirrored_);
        frameCallBack_(newFrame);
    }
}


