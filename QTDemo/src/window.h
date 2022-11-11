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
#include <QLabel>

#include "multimedia/video_capture.h"
#include "multimedia/video_widget.h"

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

private slots:
    void ClickDevicesButton();

private:
    VideoCaputer videoCapture_;
    QPushButton *captureButton_;
    QScopedPointer<VideoWidget> videoWidget_;
    QScopedPointer<QLabel> imageView_;
};

}

#endif /* !WINDOW_H */
