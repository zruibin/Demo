/*
 * right_controller.h
 *
 * Created by Ruibin.Chow on 2022/11/11.
 * Copyright (c) 2022年 Ruibin.Chow All rights reserved.
 */

#ifndef RIGHT_CONTROLLER_H
#define RIGHT_CONTROLLER_H

#include <QWidget>
#include <QScopedPointer>
#include <QHBoxLayout>

namespace UI {

class RightController : public QWidget {

Q_OBJECT

public:
    RightController(QWidget* parent = nullptr);
    ~RightController() = default;
protected:
    void customEvent(QEvent *event) override {};
private:
    QScopedPointer<QHBoxLayout> layout_;
};

}

#endif /* !RIGHT_CONTROLLER_H */
