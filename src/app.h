/*
 * app.h
 *
 * Created by Ruibin.Chow on 2022/06/14.
 * Copyright (c) 2022年 Ruibin.Chow All rights reserved.
 */

#ifndef APP_H
#define APP_H

#include <QMainWindow>

class QWidget;
class QAction;
class QMenu;

class MainWindow : public QMainWindow
{

Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow() = default;

protected:
    QWidget *pCenterWidget_;
    QAction *openAction_; //定义一个动作
    QMenu *fileMenu_; //定义一个菜单

private slots:
    void ClickButton();

};


#endif /* !APP_H */
