/*
 *
 * left_controller.cc
 *
 * Created by Ruibin.Chow on 2022/11/11.
 * Copyright (c) 2022年 Ruibin.Chow All rights reserved.
 */

#include "controller/left_controller.h"
#include <QDebug>
#include <QPushButton>
#include <QApplication>
#include "foundation/notification_center.h"

namespace UI {

LeftController::LeftController(QWidget* parent) : QWidget(parent) {
    qDebug() << "LeftController::LeftController.";
//    this->setStyleSheet("background-color:rgb(0,0,0);");

    QPushButton* button = new QPushButton;
    button->setText("Left");
    button->setStyleSheet("QPushButton{font-size:13px;color:red;}");
    button->setGeometry(QRect(0, 0, 100, 40));
    button->clearMask();
    connect(button, SIGNAL(clicked()), this, SLOT(OnOpacityAnimationBtnClicked()));
    button->setBackgroundRole(QPalette::Base);


    layout_.reset(new QHBoxLayout());
    layout_->setSpacing(0);
    layout_->setContentsMargins(0, 0, 0, 0);
    layout_->addWidget(button);
    this->setLayout(layout_.get());

    animation_.reset(new QPropertyAnimation);
    animation_->setTargetObject(button);    //设置使用动画的控件
    animation_->setEasingCurve(QEasingCurve::Linear);//设置动画效果
}

void LeftController::OnOpacityAnimationBtnClicked() {
    qDebug() << "LeftController::OnOpacityAnimationBtnClicked.";
    //windowOpacity：不透明度（注意该效果只对顶级窗口有效哦）
//    animation_->setTargetObject(this);     //重设动画使用对象
    animation_->setPropertyName("windowOpacity");  //指定动画属性名
    animation_->setDuration(1000);     //设置动画时间（单位：毫秒）

    //设置动画步长值，以及在该位置时显示的透明度
    animation_->setKeyValueAt(0, 1);
    animation_->setKeyValueAt(0.5, 0);
    animation_->setKeyValueAt(1, 0);

    animation_->setLoopCount(-1);  //当值为-1时，动画一直运行，直到窗口关闭
    animation_->start();   //启动动画
}

}

