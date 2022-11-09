/*
 *
 * video_widget.cc
 *
 * Created by Ruibin.Chow on 2022/11/04.
 * Copyright (c) 2022年 Ruibin.Chow All rights reserved.
 */

#include "video_widget.h"

VideoWidget::VideoWidget() {
    videoRender_.reset(new VideoRender);
}

void VideoWidget::PlayFrame(const QVideoFrame& frame) {
    if (!frame.isValid()) {
        qDebug()<< "VideoWidget::PlayFrame->frame was not valid";
        return ;
    }

//    qDebug()<< "VideoWidget::PlayFrame->frame reset.";
    videoFrame_.reset(new QVideoFrame(frame));
    // 刷新界面,触发paintGL接口
    update();
}

void VideoWidget::initializeGL() {
    videoRender_->Initialize();
}

void VideoWidget::resizeGL(int w, int h) {
    glViewport(0, 0, w, h);
}

void VideoWidget::paintGL() {
    if (videoFrame_ == nullptr) {
        qDebug()<< "VideoWidget::paintGL->frame was null.";
        return ;
    }

    QVideoFrameFormat::PixelFormat pixelFormat = videoFrame_->pixelFormat();
    if (pixelFormat != QVideoFrameFormat::Format_NV12) {
        qDebug()<< "VideoWidget::paintGL->frame was "
                << QVideoFrameFormat::pixelFormatToString(pixelFormat);
        return ;
    }

    if (videoFrame_->planeCount() != 2) {
        qDebug()<< "VideoWidget::paintGL->planeCount was not 2.";
        return ;
    }

    videoFrame_->map(QVideoFrame::ReadOnly);
//    qDebug()<< "VideoWidget::paintGL->frame Render.";
    videoRender_->Render(videoFrame_->textureHandle(0),
                         videoFrame_->textureHandle(1));
//    videoRender_->Render(videoFrame_->bits(0),
//                         videoFrame_->bits(1),
//                         videoFrame_->width(),
//                         videoFrame_->height());
    videoFrame_->unmap();
}
