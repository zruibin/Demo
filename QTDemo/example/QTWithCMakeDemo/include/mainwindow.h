#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "globalconfig.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_actionEnglish_triggered();

    void on_actionChinese_triggered();

    void ReTranslate(Language language);
    void on_pushButton_clicked();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
