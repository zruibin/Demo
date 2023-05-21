/*
 * left_controller.h
 *
 * Created by Ruibin.Chow on 2022/11/11.
 * Copyright (c) 2022年 Ruibin.Chow All rights reserved.
 */

#ifndef LEFT_CONTROLLER_H
#define LEFT_CONTROLLER_H

#include <QWidget>
#include <QScopedPointer>
#include <QVBoxLayout>
#include <QPushButton>
#include <QPropertyAnimation>

namespace UI {

class LeftController : public QWidget {

Q_OBJECT

public:
    LeftController(QWidget* parent = nullptr);
    ~LeftController() = default;
protected slots:
    void OnOpacityAnimationBtnClicked();
    void resizeEvent(QResizeEvent *event) override;
private:
    QScopedPointer<QVBoxLayout> layout_;
    QScopedPointer<QPushButton> testButton1_;
    QScopedPointer<QPushButton> testButton2_;
    QScopedPointer<QPropertyAnimation> animation_;
};

}


#endif /* !LEFT_CONTROLLER_H */
