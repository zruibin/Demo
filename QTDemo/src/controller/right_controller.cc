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
#include "foundation/notification_center.h"

namespace UI {

RightController::RightController(QWidget* parent) : QWidget(parent) {
    qDebug() << "RightController::RightController.";
//    this->setStyleSheet("background-color:rgb(0,1,0);");

    QPushButton* button = new QPushButton;
    button->setText("Right");
    button->setStyleSheet("QPushButton{font-size:13px;color:red;}");
    button->setGeometry(QRect(0, 0, 100, 40));
    button->clearMask();
    button->setBackgroundRole(QPalette::Base);


    layout_.reset(new QHBoxLayout());
    layout_->setSpacing(0);
    layout_->setContentsMargins(0, 0, 0, 0);
    layout_->addWidget(button);
    this->setLayout(layout_.get());
}

}


