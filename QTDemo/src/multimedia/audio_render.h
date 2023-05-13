/*
 * audio_render.h
 *
 * Created by Ruibin.Chow on 2023/05/13.
 * Copyright (c) 2023年 Ruibin.Chow All rights reserved.
 */

#ifndef AUDIO_RENDER_H
#define AUDIO_RENDER_H

#include <QAudioInput>
#include <QAudioDevice>
#include <QAudioFormat>
#include <QAudioSink>
#include <QScopedPointer>
#include <stdio.h>

class AudioRender : public QObject {
    Q_OBJECT
public:
    explicit AudioRender();
    virtual ~AudioRender();

    void Init();
    void Start();
    void Stop();
    bool IsRunning() const { return running_; };
    bool IsInit() const { return initialize_; };

private:
    
    QScopedPointer<QAudioSink> audioSink_;
    FILE *file_;
    QScopedPointer<QString> filePath_;
    
    bool running_ = false;
    bool initialize_ = false;
};

#endif /* !AUDIO_RENDER_H */
