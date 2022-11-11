/*
 *
 * notification_center.cc
 *
 * Created by Ruibin.Chow on 2022/11/11.
 * Copyright (c) 2022年 Ruibin.Chow All rights reserved.
 */

#include "notification_center.h"


namespace Foundation {

void NotificationCenter::AddObserver(const QObject* observer,
                                     const String& aName,
                                     SelectorHandler selectorHandler) {
    if (observer == nullptr) return;
    if (aName.empty()) return;
    if (selectorHandler == nullptr) return;

    String observerHashCodeKey = observer->objectName().toStdString();

    Map observerMap;
    SelectorHandlerList selectorHandlerList;

    auto it = notificationMap_->find(aName);
    if (it != notificationMap_->end()) {
        observerMap = it->second;
        selectorHandlerList = observerMap.find(observerHashCodeKey)->second;
    }

    selectorHandlerList.push_back(selectorHandler);
    observerMap.emplace(observerHashCodeKey, selectorHandlerList);
    notificationMap_->emplace(aName, observerMap);
}

void NotificationCenter::PostNotification(const String& aName,
                                          NSNotificationRef object) {
    if (aName.empty()) return;

    auto it = notificationMap_->find(aName);
    if (it == notificationMap_->end()) {
        return;
    }
    Map observerMap = it->second;
    for(auto observerMapIt = observerMap.begin(); observerMapIt != observerMap.end(); ++observerMapIt) {
        String observerHashCodeKey = observerMapIt->first;
        if (observerHashCodeKey.empty()) continue;

        SelectorHandlerList selectorHandlers = observerMapIt->second;
        for (auto vectorIt = selectorHandlers.begin(); vectorIt != selectorHandlers.end(); ++vectorIt) {
            SelectorHandler selectorHandler = *vectorIt;
            if (selectorHandler != nullptr) {
                selectorHandler(object);
            }

        }
    }
}

void NotificationCenter::PostNotification(const String& aName) {
    PostNotification(aName, nullptr);
}

void NotificationCenter::RemoveObserver(const QObject* observer,
                                        const String& aName) {
    if (observer == nullptr) return;
    if (aName.empty()) return;

    auto it = notificationMap_->find(aName);
    if (it == notificationMap_->end()) {
        return;
    }
    Map observerMap = it->second;
    String observerHashCodeKey = observer->objectName().toStdString(); //???
    auto observerIt = observerMap.find(observerHashCodeKey);
    if (observerIt != observerMap.end()) {
        observerMap.erase(observerHashCodeKey);
    }
}


}
