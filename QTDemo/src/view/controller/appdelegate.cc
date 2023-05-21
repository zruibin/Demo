/*
 *
 * appdelegate.cc
 *
 * Created by Ruibin.Chow on 2022/11/11.
 * Copyright (c) 2022年 Ruibin.Chow All rights reserved.
 */

#include "appdelegate.h"
#include <QPushButton>


namespace UI {

AppDelegate::AppDelegate(QWidget *parent) : QMainWindow(parent) {
    this->setFixedSize(800, 500);
//    setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint);//去掉标题栏
//    setAttribute(Qt::WA_TranslucentBackground, true);//设置窗口背景透明
    setWindowOpacity(0.98);
    setMouseTracking(true);
    setMaximumSize(QSize(16777215, 16777215));

    leftController.reset(new LeftController);
    rightController.reset(new RightController);

    pSplitter_.reset(new QSplitter(Qt::Horizontal, this));
    pSplitter_->setStyleSheet("QSplitter::handle{ background-color:rgb(0.13,0.63,0.13)}"); //设置分界线的样式
    pSplitter_->setHandleWidth(1);
    pSplitter_->addWidget(leftController.get());
    pSplitter_->addWidget(rightController.get());
    pSplitter_->setChildrenCollapsible(false);
    QList<int> sizes;
    sizes << 20000 << 80000;
    pSplitter_->setSizes(sizes);
    
    leftController->setMaximumWidth(200);
    leftController->setMinimumWidth(80);

    centerWidget_.reset(new QWidget);
    layout_.reset(new QHBoxLayout());
    layout_->setSpacing(0);
    layout_->setContentsMargins(0, 0, 0, 0);
    layout_->addWidget(pSplitter_.get());
    centerWidget_->setLayout(layout_.get());
    this->setCentralWidget(centerWidget_.get());
}

}
