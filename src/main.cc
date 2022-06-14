/*
 *
 * main.cc
 *
 * Created by Ruibin.Chow on 2022/06/14.
 * Copyright (c) 2022年 Ruibin.Chow All rights reserved.
 */

#include <iostream>

#include <QApplication>
#include <QTextCodec>
#include "app.h"


int main(int argc, char *argv[]) {
    std::cout << "Hello world." << std::endl;

    QApplication::setAttribute(Qt::AA_UseOpenGLES);
    QApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));

    QApplication app(argc, argv);

    app.setApplicationName("QTDemo");
    app.setApplicationVersion("V1.0.0.0");
    // app.setApplicationDisplayName(("author: ruibin.chow@qq.com"));

    MainWindow w;
    w.show();
    
    app.exec();

    return 0;
}


