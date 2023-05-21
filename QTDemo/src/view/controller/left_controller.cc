/*
 *
 * left_controller.cc
 *
 * Created by Ruibin.Chow on 2022/11/11.
 * Copyright (c) 2022年 Ruibin.Chow All rights reserved.
 */

#include "left_controller.h"
#include <QDebug>
#include <QPushButton>
#include <QResizeEvent>
#include <QFile>
#include <QFont>
#include <QFontDatabase>
#include "foundation/foundation.h"
#include "foundation/assets.h"

namespace UI {

LeftController::LeftController(QWidget* parent) : QWidget(parent) {
    qDebug() << "LeftController::LeftController.";
//    this->setStyleSheet("background-color:rgb(0,0,0);");
//    this->setFixedSize(160, 500);
//    this->setMaximumSize(QSize(16777215, 16777215));
//    this->setMinimumSize(QSize(10, 10));
    QColor color(100, 238, 100);;
    QPalette pal(color);
    this->setAutoFillBackground(true);
    this->setPalette(pal);
    
    using namespace Foundation;
    std::string stylePath = Assets::CssDirPath("style.css");
    qDebug() << "stylePath: " << stylePath.c_str();
    QFile file(QString(stylePath.c_str()));
    QString style;
    if (file.open(QFile::ReadOnly)) {
        style = QLatin1String(file.readAll());
        qDebug() << "style: " << style.toStdString().c_str();
    }
    
    testButton1_.reset(new QPushButton(this));
    testButton1_->setText("Left");
    testButton1_->setObjectName("Left1");
    testButton1_->setStyleSheet(style);
    testButton1_->setAutoFillBackground(true);
    testButton1_->setGeometry(QRect(0, 0, 100, 20));
    testButton1_->clearMask();
    connect(testButton1_.get(), SIGNAL(clicked()), this, SLOT(OnOpacityAnimationBtnClicked()));
    testButton1_->setBackgroundRole(QPalette::Button);
    testButton1_->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    animation_.reset(new QPropertyAnimation);
    animation_->setTargetObject(testButton1_.get());    //设置使用动画的控件
    animation_->setEasingCurve(QEasingCurve::Linear);//设置动画效果
    
    int fontId = QFontDatabase::addApplicationFont(Assets::FontFamily("themissinglink.ttf").c_str());
    QStringList fontFamilies = QFontDatabase::applicationFontFamilies(fontId);
    QFont font(fontFamilies.at(0).toStdString().c_str());
    
    testButton2_.reset(new QPushButton(this));
    testButton2_->setText("Left2");
    testButton2_->setGeometry(QRect(0, testButton1_->y()+testButton1_->height(), 100, 30));
    testButton2_->setObjectName("Left2");
    testButton2_->setStyleSheet(style);
    testButton2_->setFont(font);
    testButton2_->setAutoFillBackground(true);
    testButton2_->setBackgroundRole(QPalette::Button);
    testButton2_->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    qDebug() << "y:" << testButton1_->y();
    qDebug() << "height:" <<testButton1_->height();
//    layout_.reset(new QVBoxLayout());
//    layout_->setSpacing(0);
//    layout_->setContentsMargins(0, 0, 0, 0);
//    layout_->setDirection(QBoxLayout::TopToBottom);
//    layout_->setAlignment(Qt::AlignTop);
//    layout_->addWidget(testButton1_.get());
//    layout_->addWidget(testButton2_.get());
//    this->setLayout(layout_.get());
    
    using namespace Foundation;
    NotificationCenter::DefaultCenter()->AddObserver(this, "alert", [](NotificationRef noti){
        qDebug() << "LeftController::Observer->Alert";
    });
    NotificationCenter::DefaultCenter()->AddObserver(this, "alert2", [](NotificationRef noti){
        qDebug() << "LeftController::Observer->Alert2";
    });
    NotificationCenter::DefaultCenter()->AddNotification("alert", [](NotificationRef noti){
        qDebug() << "Notification->Alert->1";
    });
    NotificationCenter::DefaultCenter()->AddNotification("alert", [](NotificationRef noti){
        qDebug() << "Notification->Alert->2";
    });
    NotificationCenter::DefaultCenter()->AddNotification("alert2", [](NotificationRef noti){
        qDebug() << "Notification->Alert2->1";
    });
}

void LeftController::OnOpacityAnimationBtnClicked() {
    qDebug() << "LeftController::OnOpacityAnimationBtnClicked.";

    using namespace Foundation;
    NotificationCenter::DefaultCenter()->PostNotification("alert");
    NotificationCenter::DefaultCenter()->PostNotification("alert2");
    NotificationCenter::DefaultCenter()->RemoveObserver(this);
    NotificationCenter::DefaultCenter()->RemoveNotification("alert");

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

void LeftController::resizeEvent(QResizeEvent *event) {
    qDebug() << "resizeEvent, size:" << event->size()
                << " oldSize:" << event->oldSize();
    testButton1_->move((this->width()-100)/2, 0);
    testButton2_->move((this->width()-100)/2, this->height()-testButton2_->height());
}

}

