#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QApplication>
#include <QTranslator>
#include "testdialog.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle(tr("Test App"));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::ReTranslate(Language language)
{
    GlobalConfig::Instance().ChangeLanguage(language);
    ui->retranslateUi(this);
    setWindowTitle(tr("Test App"));
}


void MainWindow::on_actionEnglish_triggered()
{
    ReTranslate(English);
}

void MainWindow::on_actionChinese_triggered()
{
    ReTranslate(Chinese);
}

void MainWindow::on_pushButton_clicked()
{
    TestDialog dlg(this);
    dlg.exec();
}
