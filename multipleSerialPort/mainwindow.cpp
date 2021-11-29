#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "serialport.h"

serialPort eren;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect( serialPort::serialPortObj[0], SIGNAL(readRead()), this, SLOT(serialPortReadTransition(0)));
    connect( serialPort::serialPortObj[1], SIGNAL(readRead()), this, SLOT(serialPortReadTransition(1)));

    serialPort::open(0);
    serialPort::open(1);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::serialPortReadTransition(int serialPortIndex)
{
    serialPort::read(serialPortIndex);
}

