/*
 *
 * app.cc
 *
 * Created by Ruibin.Chow on 2022/06/14.
 * Copyright (c) 2022年 Ruibin.Chow All rights reserved.
 */

#include "app.h"
#include <iostream>
#include <QApplication>
#include <QTextCodec>
#include "window.h"
#include "controller/appdelegate.h"

namespace UI {

void AppLication::run(int argc, char *argv[]) {
    QApplication::setAttribute(Qt::AA_UseOpenGLES);
    QApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));

    QApplication app(argc, argv);

    app.setApplicationName("QTDemo");
    app.setApplicationVersion("V1.0.0.0");
    // app.setApplicationDisplayName(("author: ruibin.chow@qq.com"));

//    Window w;
//    w.show();

    AppDelegate appDelegate;
    appDelegate.show();
    
    app.exec();
}

}
