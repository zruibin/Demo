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

    leftController_ = new LeftController;
    rightController_ = new RightController;

    pSplitter_.reset(new QSplitter(Qt::Horizontal, this));
    pSplitter_->setStyleSheet("QSplitter::handle{ background-color:rgb(0.13,0.63,0.13)}"); //设置分界线的样式
    pSplitter_->setHandleWidth(1);
    pSplitter_->addWidget(leftController_);
    pSplitter_->addWidget(rightController_);
    pSplitter_->setChildrenCollapsible(false);
    QList<int> sizes;
    sizes << 20000 << 80000;
    pSplitter_->setSizes(sizes);
    
    leftController_->setMaximumWidth(200);
    leftController_->setMinimumWidth(80);

    QWidget* centerWidget = new QWidget;
    QHBoxLayout* layout = new QHBoxLayout();
    layout->setSpacing(0);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(pSplitter_.get());
    centerWidget->setLayout(layout);
    this->setCentralWidget(centerWidget);
}

}
