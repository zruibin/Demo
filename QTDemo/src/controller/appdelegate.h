/*
 * appdelegate.h
 *
 * Created by Ruibin.Chow on 2022/11/11.
 * Copyright (c) 2022年 Ruibin.Chow All rights reserved.
 */

#ifndef APPDELEGATE_H
#define APPDELEGATE_H

#include <QMainWindow>
#include <QScopedPointer>
#include <QHBoxLayout>
#include <QSplitter>
#include "controller/left_controller.h"
#include "controller/right_controller.h"

namespace UI {

class AppDelegate : public QMainWindow {

Q_OBJECT

public:
    AppDelegate(QWidget *parent = nullptr);
    virtual ~AppDelegate() = default;

private:
    QScopedPointer<LeftController> leftController;
    QScopedPointer<RightController> rightController;
    QScopedPointer<QHBoxLayout> layout_;
    QScopedPointer<QSplitter> pSplitter_;
    QScopedPointer<QWidget> centerWidget_;
};

}

#endif /* !APPDELEGATE_H */
