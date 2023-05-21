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


class Permission {
public:
    using PermissionFunc = std::function<void (const Permission &)>;
    
    enum class Status {
        Undetermined,
        Granted,
        Denied
    };
    
    static std::shared_ptr<Permission>& GetInstance();
    
    Status CheckCameraPermission();
    void RequestCameraPermission(PermissionFunc);
    Status CheckMicrophonePermission();
    void RequestMicrophonePermission(PermissionFunc);
    
public:
    Status status() const { return status_;};
    const char* statusString() const;

private:
    Status status_;
};


#endif /* !PERMISSION_H */
