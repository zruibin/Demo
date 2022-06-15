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
    button->setText("普通按钮");
    button->setStyleSheet("QPushButton{ font-family:'Microsoft YaHei';font-size:16px;color:red;}");
    // button->move(10, 20);
    // button->setFixedSize(100,40);
    button->setGeometry(QRect(10, 20, 100, 40));
    button->clearMask();
    button->setBackgroundRole(QPalette::Base);
    connect(button, SIGNAL(clicked()), this, SLOT(ClickButton()));
    button->setParent(pCenterWidget_);

    QPushButton *button2 = new QPushButton;
    button2->setText("普通按钮2");
    button2->setStyleSheet("QPushButton{ font-family:'Microsoft YaHei';font-size:16px;color:#000;}");
    button2->setGeometry(QRect(10, 60, 100, 40));
    button2->clearMask();
    button2->setBackgroundRole(QPalette::Base);
    connect(button2, SIGNAL(clicked()), this, SLOT(ClickButton()));
    button2->setParent(pCenterWidget_);
}

void MainWindow::ClickButton() {
    std::cout << "PushButton Click." << std::endl;
}

/*
QPushButton *custButton(QString str, QString str1)  
{  
    QPushButton *pushButton= new QPushButton;  
  
    pushButton->setGeometry(10,10,200,200); //前两个参数是位置坐标，后两个参数是按钮的尺寸。 
    pushButton->clearMask();  
    pushButton->setBackgroundRole( QPalette::Base);  
  
    QPixmap mypixmap;   
    mypixmap.load(str);  
  
    pushButton->setFixedSize(mypixmap.width(), mypixmap.height() );  
    pushButton->setMask(mypixmap.createHeuristicMask());  
    pushButton->setIcon(mypixmap);  
    pushButton->setIconSize(QSize(mypixmap.width(),mypixmap.height()));  
    pushButton->setToolTip(str1);  
    return pushButton;  
}
*/

}