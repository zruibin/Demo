/*
 *
 * assets.cc
 *
 * Created by Ruibin.Chow on 2022/11/14.
 * Copyright (c) 2022年 Ruibin.Chow All rights reserved.
 */

#include "assets.h"
#include <iostream>

namespace Foundation {

static String __staticAssetsPath{""};

String Assets::ProjectDirPath() {
#ifndef PROJECT_DIR
    return "";
#else
    return PROJECT_DIR;
#endif
}

void Assets::SetAssetsDirPath(const String& path) {
    __staticAssetsPath = path;
    std::cout << "StaticAssetsPath: " << __staticAssetsPath << std::endl;
}

String Assets::AssetsDirPath() {
    if (__staticAssetsPath.length() == 0) {
        return Assets::ProjectDirPath().append("/assets/");
    }
    return __staticAssetsPath;
}

String Assets::CssDirPath() {
    return AssetsDirPath() + "css/";
}

String Assets::CssDirPath(const String& cssName) {
    return CssDirPath() + cssName;
}

String Assets::ImagesDirPath() {
    return AssetsDirPath() + "images/";
}

String Assets::ImagesDirPath(const String& imageName) {
    return ImagesDirPath() + imageName;
}

String Assets::FontsDirPath() {
    return AssetsDirPath() + "fonts/";
}

String Assets::FontsDirPath(const String& fontName) {
    return FontsDirPath() + fontName;
}

String Assets::FontFamily(const String& fontName) {
    String fontDirPath = FontsDirPath(fontName);
//    int fontId = QFontDatabase::addApplicationFont(fontDirPath.c_str());
//    QStringList fontFamilies = QFontDatabase::applicationFontFamilies(fontId);
    return fontDirPath;//fontFamilies.at(0).toStdString();
}

String Assets::AudioDirPath() {
    return AssetsDirPath() + "audio/";
}

String Assets::AudioDirPath(const String& audioName) {
    return AudioDirPath() + audioName;
}

}
