/*
 * permission.h
 *
 * Created by Ruibin.Chow on 2023/05/13.
 * Copyright (c) 2023年 Ruibin.Chow All rights reserved.
 */

#ifndef PERMISSION_H
#define PERMISSION_H

#include <memory>
#include <functional>
#include <QApplication>
#include <QPermissions>

using PermissionFunc = std::function<void (const QPermission &)>;

class Permission {
public:
    static std::shared_ptr<Permission>& GetInstance();
    
    Qt::PermissionStatus checkCameraPermission();
    void requestCameraPermission(PermissionFunc);
    Qt::PermissionStatus checkMicrophonePermission();
    void requestMicrophonePermission(PermissionFunc);
};


#endif /* !PERMISSION_H */
