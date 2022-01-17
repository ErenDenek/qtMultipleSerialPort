#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "serialport.h"
#include <qtimer.h>
#include <QThread>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow, public serialPort
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    int count = 0;
    int count1 = 1;
    QTimer *test_send_timer;

private:
    Ui::MainWindow *ui;

public slots:
    void serialPortReadTransition1();
    void serialPortReadTransition2();
    void test_com1();
    void test_com2();
};
#endif // MAINWINDOW_H
