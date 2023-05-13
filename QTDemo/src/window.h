/*
 * window.h
 *
 * Created by Ruibin.Chow on 2022/06/15.
 * Copyright (c) 2022年 Ruibin.Chow All rights reserved.
 */

#ifndef WINDOW_H
#define WINDOW_H

#include <QMainWindow>
#include <QScopedPointer>
#include <QSharedPointer>
#include <QLabel>

#include "multimedia/video_capture.h"
#include "multimedia/video_widget.h"
#include "multimedia/audio_capture.h"

class QWidget;
class QAction;
class QMenu;
class QPushButton;


namespace UI {

class Window : public QMainWindow {

Q_OBJECT

public:
    Window(QWidget *parent = nullptr);
    ~Window() = default;

protected:
    QWidget *pCenterWidget_;
    QAction *openAction_; //定义一个动作
    QMenu *fileMenu_; //定义一个菜单

protected slots:
    void resizeEvent(QResizeEvent *event) override;

private slots:
    void ClickDevicesButton();

private:
    VideoCaputer videoCapture_;
    AudioCaputer audioCaputre_;
    QSharedPointer<QPushButton> videoButton_;
    QSharedPointer<QPushButton> audioButton_;
    QSharedPointer<QPushButton> deviceButton_;
    QScopedPointer<VideoWidget> videoWidget_;
    QScopedPointer<QLabel> imageView_;
};

}

#endif /* !WINDOW_H */
