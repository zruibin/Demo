/*
 * notification_center.h
 *
 * Created by Ruibin.Chow on 2022/11/11.
 * Copyright (c) 2022年 Ruibin.Chow All rights reserved.
 */

#ifndef NOTIFICATION_CENTER_H
#define NOTIFICATION_CENTER_H

#include <string>
#include <unordered_map>
#include <QObject>

namespace Foundation {

struct NSNotification final {
    std::string name;
    QObject object;
    //userInfo
};

class NotificationCenter final {

public:
    using NSNotificationRef = std::shared_ptr<NSNotification>;
    using SelectorHandler = std::function<void(NSNotificationRef)>;
    using String = std::string;
    using SelectorHandlerList = std::vector<SelectorHandler>;
    using Map = std::unordered_map<String, SelectorHandlerList>;
    using MapRef = std::shared_ptr<std::unordered_map<String, Map>>;

    static NotificationCenter defaultCenter() {
        return instance_;
    }

    void AddObserver(const QObject* observer,
                     const String& aName,
                     SelectorHandler selectorHandler);
    void PostNotification(const String& aName,
                          NSNotificationRef object);
    void PostNotification(const String& aName);
    void RemoveObserver(const QObject* observer,
                        const String& aName);

private:
    NotificationCenter() = default;
    NotificationCenter(NotificationCenter&) = default;

private:
    static NotificationCenter instance_;
    MapRef notificationMap_;
};

}

#endif /* !NOTIFICATION_CENTER_H */
