/*
 *
 * app.cc
 *
 * Created by Ruibin.Chow on 2022/06/14.
 * Copyright (c) 2022年 Ruibin.Chow All rights reserved.
 */

#include "app.h"
#include <iostream>
#include <filesystem>
#include "foundation/assets.h"
#if VIEW_DISPLAY
#include <QApplication>
#include <QTextCodec>
#include <QDir>
#include "view/window.h"
#include "view/controller/appdelegate.h"
#endif

namespace UI {

void AppLication::run(int argc, char *argv[]) {
    using namespace Foundation;
#if VIEW_DISPLAY
    QApplication::setAttribute(Qt::AA_UseOpenGLES);
    QApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));

    QApplication app(argc, argv);
    
    QString dirPath = QCoreApplication::applicationDirPath().append("/../../../Data");
    QDir dir(dirPath);
    if(!dir.exists()) dir.mkdir(dirPath);
    QString assetsPath = QCoreApplication::applicationDirPath().append("/../Resources/assets/");
    Assets::SetAssetsDirPath(assetsPath.toStdString());
    
    app.setApplicationName("QTDemo");
    app.setApplicationVersion("V1.0.0.0");
    // app.setApplicationDisplayName(("author: ruibin.chow@qq.com"));

    Window w;
    w.show();

//    AppDelegate appDelegate;
//    appDelegate.show();
    
    app.exec();
#else
    namespace fs = std::filesystem;
    fs::path dst = fs::current_path() / std::string("./Data");
    if (!fs::exists(dst)) fs::create_directory(dst);
    Assets::SetAssetsDirPath(Assets::ProjectDirPath().append("/assets/"));
    
    std::cout << "Run App." << std::endl;
#endif


}

}
