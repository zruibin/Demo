/*
 *
 * window.cc
 *
 * Created by Ruibin.Chow on 2022/06/15.
 * Copyright (c) 2022年 Ruibin.Chow All rights reserved.
 */

#include "window.h"
#include <iostream>
#include <QMainWindow>
#include <QWidget>
#include <QMenu>
#include <QMenuBar>
#include <QAction>
#include <QPushButton>
#include <QVBoxLayout>
#include <QResizeEvent>
#include "multimedia/devices.h"
#include "multimedia/video_capture.h"
#include "multimedia/permission.h"

namespace UI {

Window::Window(QWidget *parent) : QMainWindow(parent) {
    qDebug() << "Window, frame:" << this->rect()
                << " size:" << this->frameSize();
    this->setWindowTitle(tr("QTDemo Window"));
    // this->setWindowFlags(Qt::WindowTitleHint | 
    //                     Qt::CustomizeWindowHint | 
    //                     Qt::MSWindowsFixedSizeDialogHint);
    this->setFixedSize(800, 560);
    this->setWindowOpacity(0.95);
    this->setMouseTracking(true);
    this->setMaximumSize(QSize(16777215, 16777215));

    openAction_ = new QAction(tr("Open"),this); //初始化动作
    fileMenu_ = new QMenu; //创建一个菜单
    fileMenu_ = this->menuBar()->addMenu(tr("File")); //将菜单添加到菜单栏上
    fileMenu_->addAction(openAction_); //将动作添加到菜单上

    pCenterWidget_ = new QWidget;
    this->setCentralWidget(pCenterWidget_);//把这个空窗口设置为QMainWindow的中心窗口

    deviceButton_ = QSharedPointer<QPushButton>(new QPushButton);
    deviceButton_->setText("Devices Info");
    deviceButton_->setStyleSheet("QPushButton{ font-family:'Microsoft YaHei';font-size:13px;color:red;}");
    // button->move(10, 20);
    // button->setFixedSize(100,40);
    deviceButton_->setGeometry(QRect((this->width()-100)/2, 0, 100, 40));
    deviceButton_->clearMask();
    deviceButton_->setBackgroundRole(QPalette::Base);
    connect(deviceButton_.get(), SIGNAL(clicked()), this, SLOT(ClickDevicesButton()));
    deviceButton_->setParent(pCenterWidget_);
    
    audioButton_ = QSharedPointer<QPushButton>(new QPushButton);
    audioButton_->setText("开始音频采集");
    audioButton_->setStyleSheet("QPushButton{font-size:13px;color:#000;}");
    audioButton_->clearMask();
    audioButton_->setBackgroundRole(QPalette::Base);
    connect(audioButton_.get(), &QPushButton::clicked, this, [this](){
        Qt::PermissionStatus status = Permission::GetInstance()->CheckMicrophonePermission();
        if (status != Qt::PermissionStatus::Granted) {
            Permission::GetInstance()->RequestMicrophonePermission([this](const QPermission &permission) {
                qDebug() << "MicrophonePermission status: " << permission.status();
            });
            return ;
        }
        
        if (!audioCaputre_.IsInit()) {
            audioCaputre_.Init();
        }
        if (audioCaputre_.IsRunning()) {
            audioCaputre_.Stop();
            audioButton_->setText("开始音频采集");
        } else {
            audioCaputre_.Start();
            audioButton_->setText("停止音频采集");
        }
    });
    audioButton_->setParent(pCenterWidget_);
    
    audioRenderButton_ = QSharedPointer<QPushButton>(new QPushButton);
    audioRenderButton_->setText("开始播放音频");
    audioRenderButton_->setStyleSheet("QPushButton{font-size:13px;color:#000;}");
    audioRenderButton_->clearMask();
    audioRenderButton_->setBackgroundRole(QPalette::Base);
    connect(audioRenderButton_.get(), &QPushButton::clicked, this, [this](){
        if (!audioRender_.IsInit()) {
            audioRender_.Init();
        }
        if (audioRender_.IsRunning()) {
            audioRender_.Stop();
            audioRenderButton_->setText("开始播放音频");
        } else {
            audioRender_.Start();
            audioRenderButton_->setText("停止播放音频");
        }
    });
    audioRenderButton_->setParent(pCenterWidget_);

    videoButton_ = QSharedPointer<QPushButton>(new QPushButton);
    videoButton_->setText("开始相机采集");
    videoButton_->setStyleSheet("QPushButton{font-size:13px;color:#000;}");
    videoButton_->clearMask();
    videoButton_->setBackgroundRole(QPalette::Base);
    connect(videoButton_.get(), &QPushButton::clicked, this, [this](){
        Qt::PermissionStatus status = Permission::GetInstance()->CheckCameraPermission();
        if (status != Qt::PermissionStatus::Granted) {
            Permission::GetInstance()->RequestCameraPermission([this](const QPermission &permission) {
                qDebug() << "CameraPermission status: " << permission.status();
            });
            return;
        }
        
        if (!videoCapture_.IsInit()) {
            videoCapture_.Init();
        }
        if (videoCapture_.IsRunning()) {
            videoCapture_.Stop();
            videoButton_->setText("开始相机采集");
        } else {
            videoCapture_.Start();
            videoButton_->setText("停止相机采集");
        }
    });
    videoButton_->setParent(pCenterWidget_);

    videoCapture_.SetMirrored(true);
    videoCapture_.SetFrameCallBack([this](const QVideoFrame &frame){
        if (videoWidget_ != nullptr) {
            videoWidget_->PlayFrame(frame);
        }
        if (imageView_ != nullptr) {
            QImage scaledImage = frame.toImage().scaled(imageView_->size(),
                                                        Qt::KeepAspectRatio,
                                                        Qt::SmoothTransformation);
            imageView_->setPixmap(QPixmap::fromImage(scaledImage));
        }

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
        }
        static int frameNum = 0;
        ++frameNum;
//        qDebug("%d, %d, %d, %d, %d, %p, %d", frameNum,
//               pixelFormat, width, height, planeCount, pdata, len);

        videoFrame.unmap();
    });

    videoWidget_.reset(new VideoWidget);
    videoWidget_->clearMask();
    videoWidget_->setParent(pCenterWidget_);

    imageView_.reset(new QLabel);
    imageView_->clearMask();
    imageView_->setStyleSheet("QLabel{background-color:rgb(0,0,0);}");
    imageView_->setParent(pCenterWidget_);
    
    textArea_.reset(new QTextEdit);
    textArea_->setReadOnly(true);
    textArea_->setParent(pCenterWidget_);
    
    audioButton_->setGeometry(QRect(this->width()/2-50,
                                    deviceButton_->geometry().bottom()+5,
                                    100, 40));
    videoButton_->setGeometry(QRect(audioButton_->x()-audioButton_->width()-10,
                                    audioButton_->geometry().top(), 100, 40));
    audioRenderButton_->setGeometry(QRect(audioButton_->geometry().right()+10,
                                          audioButton_->geometry().top(), 100, 40));
    
    videoWidget_->setGeometry(QRect(this->width()/2-310, audioButton_->geometry().bottom()+10, 300, 200));
    imageView_->setGeometry(QRect(this->width()/2+10, videoWidget_->geometry().top(), 300, 200));
    
    textArea_->setGeometry(QRect(10,
                                 imageView_->geometry().bottom()+10,
                                 this->width()-20,
                                 this->height()-imageView_->geometry().bottom()-20));
}

void Window::ClickDevicesButton() {
    std::cout << "PushButton Click." << std::endl;
    QString info = DevicesMain();
    qDebug() << "Devices info: " << info;
    textArea_->setText(info);
}

void Window::resizeEvent(QResizeEvent *event) {
    qDebug() << "Window resizeEvent, size:" << event->size()
                << " oldSize:" << event->oldSize();
    qDebug() << "Window resizeEvent, frame:" << this->rect()
                << " size:" << this->frameSize();
    deviceButton_->move((this->width()-100)/2, deviceButton_->geometry().top());
    
    audioButton_->move(this->width()/2-50,
                       deviceButton_->geometry().bottom()+5);
    videoButton_->move(audioButton_->x()-audioButton_->width()-10,
                       audioButton_->geometry().top());
    audioRenderButton_->move(audioButton_->geometry().right()+10,
                             audioButton_->geometry().top());
    
    videoWidget_->move(this->width()/2-310, audioButton_->geometry().bottom()+10);
    imageView_->move(this->width()/2+10, videoWidget_->geometry().top());
    
    textArea_->setGeometry(QRect(10,
                                 imageView_->geometry().bottom()+10,
                                 this->width()-20,
                                 this->height()-imageView_->geometry().bottom()-20));
}

}
