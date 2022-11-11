/*
 *
 * appdelegate.cc
 *
 * Created by Ruibin.Chow on 2022/11/11.
 * Copyright (c) 2022年 Ruibin.Chow All rights reserved.
 */

#include "controller/appdelegate.h"
#include <QSplitter>
#include <QPushButton>


namespace UI {

AppDelegate::AppDelegate(QWidget *parent)
        : QMainWindow(parent) {
    this->setFixedSize(800, 500);
//    setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint);//去掉标题栏
//    setAttribute(Qt::WA_TranslucentBackground, true);//设置窗口背景透明
    setWindowOpacity(0.98);
    setMouseTracking(true);
    setMaximumSize(QSize(16777215, 16777215));

    leftController.reset(new LeftController);
    rightController.reset(new RightController);

    QSplitter* pSplitter = new QSplitter(Qt::Horizontal, this);
    pSplitter->setStyleSheet("QSplitter::handle{ background-color:rgb(0.13,0.63,0.13)}"); //设置分界线的样式
    pSplitter->setHandleWidth(1);
    pSplitter->addWidget(leftController.get());
    pSplitter->addWidget(rightController.get());
    pSplitter->setStretchFactor(0, 2);
    pSplitter->setStretchFactor(1, 8);


    centerWidget_.reset(new QWidget);
    layout_.reset(new QHBoxLayout());
    layout_->setSpacing(0);
    layout_->setContentsMargins(0, 0, 0, 0);
    layout_->addWidget(pSplitter);
    centerWidget_->setLayout(layout_.get());
    this->setCentralWidget(centerWidget_.get());
}

}
