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
#if __has_include(<QPermissions>)
#include <QPermissions>
#define HAS_PERMISSION
#endif

#if defined(HAS_PERMISSION)
static Permission::Status PermissionConvert(Qt::PermissionStatus qStatus) {
    Permission::Status status;
    switch (qStatus) {
        case Qt::PermissionStatus::Granted:
            status = Permission::Status::Granted;
            break;
        case Qt::PermissionStatus::Denied:
            status = Permission::Status::Denied;
            break;
        default:
            status = Permission::Status::Undetermined;
            break;
    }
    return status;
}
#endif

std::shared_ptr<Permission>& Permission::GetInstance() {
    static std::shared_ptr<Permission> instance_;
    static std::once_flag flag;
    std::call_once(flag, [](){
        instance_.reset(new Permission);
    });
    return instance_;
}

Permission::Status Permission::CheckCameraPermission() {
#if defined(HAS_PERMISSION)
    Qt::PermissionStatus qStatus = qApp->checkPermission(QCameraPermission{});
    return PermissionConvert(qStatus);
#else
    return Permission::Status::Granted;
#endif
}

void Permission::RequestCameraPermission(PermissionFunc func) {
#if defined(HAS_PERMISSION)
    qApp->requestPermission(QCameraPermission{}, [this, func](const QPermission &permission) {
        if (func) {
            this->status_ = PermissionConvert(permission.status());
            func(*this);
        }
    });
#else
    if (func) {
        this->status_ = Permission::Status::Granted;
        func(*this);
    }
#endif
}

Permission::Status Permission::CheckMicrophonePermission() {
#if defined(HAS_PERMISSION)
    Qt::PermissionStatus qStatus = qApp->checkPermission(QMicrophonePermission{});
    return PermissionConvert(qStatus);
#else
    return Permission::Status::Granted;
#endif
}

void Permission::RequestMicrophonePermission(PermissionFunc func) {
#if defined(HAS_PERMISSION)
    qApp->requestPermission(QMicrophonePermission{}, [this, func](const QPermission &permission) {
        if (func) {
            this->status_ = PermissionConvert(permission.status());
            func(*this);
        }
    });
#else
    if (func) {
        this->status_ = Permission::Status::Granted;
        func(*this);
    }
#endif
}

const char* Permission::statusString() const {
    switch (status_) {
        case Permission::Status::Granted:
            return "Granted";
            break;
        case Permission::Status::Denied:
            return "Denied";
            break;
        default:
            return "Undetermined";
            break;
    }
}
