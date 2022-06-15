/*
 * window.h
 *
 * Created by Ruibin.Chow on 2022/06/15.
 * Copyright (c) 2022年 Ruibin.Chow All rights reserved.
 */

#ifndef WINDOW_H
#define WINDOW_H

#include <QMainWindow>

class QWidget;
class QAction;
class QMenu;

namespace UI {

class MainWindow : public QMainWindow {

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

}

#endif /* !WINDOW_H */
