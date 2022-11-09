#include "mainwindow.h"

#include <QApplication>
#include <QTextCodec>
#include "globalconfig.h"

int main(int argc, char *argv[])
{
    QApplication::setAttribute(Qt::AA_UseOpenGLES);
    QApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));

    QApplication a(argc, argv);

    GlobalConfig::Instance().LoadLanguage();
    GlobalConfig::Instance().ChangeLanguage(Chinese);

    a.setApplicationName("Tesp App");
    a.setApplicationVersion("V1.0.0.0");
    // a.setApplicationDisplayName(("author: yuanfeng1897@gmail.com"));

    MainWindow w;
    w.show();
    
    a.exec();    
    return 0;
}
