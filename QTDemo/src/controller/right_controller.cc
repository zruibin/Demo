/*
 *
 * right_controller.cc
 *
 * Created by Ruibin.Chow on 2022/11/11.
 * Copyright (c) 2022年 Ruibin.Chow All rights reserved.
 */

#include "controller/right_controller.h"
#include <QDebug>
#include <QPushButton>
#include <QPalette>
#include "foundation/foundation.h"

namespace UI {

RightController::RightController(QWidget* parent) : QWidget(parent) {
    qDebug() << "RightController::RightController.";
//    this->setStyleSheet("background-color:rgb(0,1,0);");
//    this->setFixedSize(640, 500);
//    this->setMaximumSize(QSize(16777215, 16777215));
    
    QColor color(100, 100, 238);;
    QPalette pal(color);
    this->setAutoFillBackground(true);
    this->setPalette(pal);

    QPushButton* button = new QPushButton(this);
    button->setText("Right");
    button->setStyleSheet("QPushButton{font-size:11px;color:red;background-color:#FFF}");
    button->setGeometry(QRect(10, 10, 100, 40));
    button->clearMask();
    button->setBackgroundRole(QPalette::Base);


//    layout_.reset(new QHBoxLayout());
//    layout_->setSpacing(0);
//    layout_->setContentsMargins(0, 0, 0, 0);
//    layout_->addWidget(button);
//    this->setLayout(layout_.get());

    using namespace Foundation;
    NotificationCenter::DefaultCenter()->AddObserver(this, "alert", [button](NotificationRef noti){
        qDebug() << "RightController::Observer->Alert";
        button->hide();
    });
    NotificationCenter::DefaultCenter()->AddObserver(this, "alert2", [](NotificationRef noti){
        qDebug() << "RightController::Observer->Alert2";
    });
}

}


