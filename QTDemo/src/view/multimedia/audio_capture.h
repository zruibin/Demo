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
#include <QMediaFormat>
#include <QAudioSource>
#include <QMediaRecorder>
#include <QScopedPointer>
#include <QAudioBuffer>
#include <QFile>

class AudioProbeDevice : public QIODevice {
    Q_OBJECT
    QAudioFormat m_format;
public:
    AudioProbeDevice(QObject* parent = {}) : QIODevice(parent) {}
    void start(QAudioSource *source) {
        Q_ASSERT(source);
        m_format = source->format();
        open(QIODevice::WriteOnly);
    }
    
    void stop() {
        close();
    }
    
    qreal calculateLevel(const char *data, qint64 len) {
        const int channelBytes = m_format.bytesPerSample();
        const int sampleBytes = m_format.bytesPerFrame();
        const int numSamples = len / sampleBytes;

        float maxValue = 0;
        auto *ptr = reinterpret_cast<const unsigned char *>(data);

        for (int i = 0; i < numSamples; ++i) {
            for (int j = 0; j < m_format.channelCount(); ++j) {
                float value = m_format.normalizedSampleValue(ptr);

                maxValue = qMax(value, maxValue);
                ptr += channelBytes;
            }
        }
        emit OnLevelChanged(maxValue);
        return maxValue;
    }
    
    qint64 readData(char *, qint64) override { return 0; }
    qint64 writeData(const char *data, qint64 count) override {
        qDebug() << "writeData: " << count;
        QAudioBuffer buffer({data, static_cast<int>(count)}, m_format);
        emit OnBufferChanged(buffer);
        calculateLevel(data, count);
        return count;
    }
    
    Q_SIGNAL void OnBufferChanged(const QAudioBuffer &buffer);
    Q_SIGNAL void OnLevelChanged(const float level);
};


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
//    QMediaCaptureSession captureSession_;
//    QScopedPointer<QAudioInput> audioInput_;
//    QScopedPointer<QMediaRecorder> audioRecorder_;
    
    QScopedPointer<QAudioSource> audioSource_;
    QScopedPointer<AudioProbeDevice> audioProbe_;
    QScopedPointer<QFile> file_;
    QScopedPointer<QString> filePath_;
    
    bool running_ = false;
    bool initialize_ = false;
};

#endif /* !AUDIO_CAPTURE_H */
