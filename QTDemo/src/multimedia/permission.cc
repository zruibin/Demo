/*
 *
 * permission.cc
 *
 * Created by Ruibin.Chow on 2023/05/13.
 * Copyright (c) 2023年 Ruibin.Chow All rights reserved.
 */

#include "permission.h"
#include <mutex>
#include <QApplication>

std::shared_ptr<Permission>& Permission::GetInstance() {
    static std::shared_ptr<Permission> instance_;
    static std::once_flag flag;
    std::call_once(flag, [](){
        instance_.reset(new Permission);
    });
    return instance_;
}

Qt::PermissionStatus Permission::checkCameraPermission() {
    return qApp->checkPermission(QCameraPermission{});
}

void Permission::requestCameraPermission(PermissionFunc func) {
    qApp->requestPermission(QCameraPermission{}, [func](const QPermission &permission) {
        if (func) {
            func(permission);
        }
    });
}

Qt::PermissionStatus Permission::checkMicrophonePermission() {
    return qApp->checkPermission(QMicrophonePermission{});
}

void Permission::requestMicrophonePermission(PermissionFunc func) {
    qApp->requestPermission(QMicrophonePermission{}, [func](const QPermission &permission) {
        qDebug() << "MicrophonePermission status: " << permission.status();
        if (func) {
            func(permission);
        }
    });
}

