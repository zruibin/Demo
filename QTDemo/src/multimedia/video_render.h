/*
 * video_render.h
 *
 * Created by Ruibin.Chow on 2022/11/04.
 * Copyright (c) 2022年 Ruibin.Chow All rights reserved.
 */

#ifndef VIDEO_RENDER_H
#define VIDEO_RENDER_H

#include <QVideoFrame>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>

class VideoRender : public QOpenGLFunctions {
//    Q_OBJECT
public:
    VideoRender() = default;
    VideoRender(const VideoRender&) = delete;
    ~VideoRender() = default;

    void Initialize();
    void Render(uchar* m_py,uchar* m_pu, int width, int height);
    void Render(quint64 textureY, quint64 textureUV);
    void Render(uchar* nv12Ptr, int w, int h);

private:
    QOpenGLShaderProgram program;
    GLuint idY, idUV;
    QOpenGLBuffer vbo;
};


#endif /* !VIDEO_RENDER_H */
