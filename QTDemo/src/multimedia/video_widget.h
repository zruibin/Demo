/*
 * video_widget.h
 *
 * Created by Ruibin.Chow on 2022/11/04.
 * Copyright (c) 2022年 Ruibin.Chow All rights reserved.
 */

#ifndef VIDEO_WIDGET_H
#define VIDEO_WIDGET_H

#include <QScopedPointer>
#include <QOpenGLWidget>
#include <QVideoFrame>
#include "video_render.h"

class VideoWidget : public QOpenGLWidget {
    Q_OBJECT
public:
    explicit VideoWidget();
    VideoWidget(const VideoWidget&) = delete;
    ~VideoWidget() = default;
    void PlayFrame(const QVideoFrame& frame);

protected:
    void initializeGL() Q_DECL_OVERRIDE;
    void resizeGL(int w, int h) Q_DECL_OVERRIDE;
    void paintGL() Q_DECL_OVERRIDE;

private:
    QScopedPointer<VideoRender> videoRender_;
    QScopedPointer<QVideoFrame> videoFrame_;
};


#endif /* !VIDEO_WIDGET_H */
