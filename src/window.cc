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

#include "multimedia/devices.h"
#include "multimedia/video_capture.h"

namespace UI {

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent) {
    this->setWindowTitle(tr("QTDemo Window"));
    // this->setWindowFlags(Qt::WindowTitleHint | 
    //                     Qt::CustomizeWindowHint | 
    //                     Qt::MSWindowsFixedSizeDialogHint);
    this->setFixedSize(800, 560);

    openAction_ = new QAction(tr("Open"),this); //初始化动作
    fileMenu_ = new QMenu; //创建一个菜单
    fileMenu_ = this->menuBar()->addMenu(tr("File")); //将菜单添加到菜单栏上
    fileMenu_->addAction(openAction_); //将动作添加到菜单上

    pCenterWidget_ = new QWidget;
    this->setCentralWidget(pCenterWidget_);//把这个空窗口设置为QMainWindow的中心窗口

    QPushButton *button = new QPushButton;
    button->setText("DevicesMain");
    button->setStyleSheet("QPushButton{ font-family:'Microsoft YaHei';font-size:13px;color:red;}");
    // button->move(10, 20);
    // button->setFixedSize(100,40);
    button->setGeometry(QRect(10, 20, 100, 40));
    button->clearMask();
    button->setBackgroundRole(QPalette::Base);
    connect(button, SIGNAL(clicked()), this, SLOT(ClickDevicesButton()));
    button->setParent(pCenterWidget_);


    captureButton_ = new QPushButton;
    captureButton_->setText("开始相机采集");
    captureButton_->setStyleSheet("QPushButton{font-size:13px;color:#000;}");
    captureButton_->setGeometry(QRect(10, 60, 100, 40));
    captureButton_->clearMask();
    captureButton_->setBackgroundRole(QPalette::Base);
    connect(captureButton_, &QPushButton::clicked, this, [this](){
        if (!videoCapture_.IsInit()) {
            videoCapture_.Init();
        }
        if (videoCapture_.IsRunning()) {
            videoCapture_.Stop();
            captureButton_->setText("开始相机采集");
        } else {
            videoCapture_.Start();
            captureButton_->setText("停止相机采集");
        }
    });
    captureButton_->setParent(pCenterWidget_);
}

void MainWindow::ClickDevicesButton() {
    std::cout << "PushButton Click." << std::endl;
    DevicesMain();
}

}
