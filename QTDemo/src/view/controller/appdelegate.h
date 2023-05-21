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
#include <QSharedPointer>
#include <QHBoxLayout>
#include <QSplitter>
#include "left_controller.h"
#include "right_controller.h"

namespace UI {

class AppDelegate : public QMainWindow {

Q_OBJECT

public:
    AppDelegate(QWidget *parent = nullptr);
    virtual ~AppDelegate() = default;

private:
    LeftController* leftController_;
    RightController* rightController_;
    QScopedPointer<QSplitter> pSplitter_;
};

}

#endif /* !APPDELEGATE_H */
