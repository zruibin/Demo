/*
 * assets.h
 *
 * Created by Ruibin.Chow on 2022/11/14.
 * Copyright (c) 2022年 Ruibin.Chow All rights reserved.
 */

#ifndef ASSETS_H
#define ASSETS_H

#include <string>

namespace Foundation {

using String = std::string;

struct Assets final {
    static String ProjectDirPath();
    
    static void SetAssetsDirPath(const String& path);
    static String AssetsDirPath();
    
    static String CssDirPath();
    static String CssDirPath(const String& cssName);
    
    static String ImagesDirPath();
    static String ImagesDirPath(const String& imageName);
    
    static String FontsDirPath();
    static String FontsDirPath(const String& fontName);
    static String FontFamily(const String& fontName);
};

}

#endif /* !ASSETS_H */
