#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "serialport.h"


bool testOrder;

serialPort serialPort1, serialPort2, serialPort3;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect( serialPort1.serialPortObj, SIGNAL(readyRead()), this, SLOT(serialPortReadTransition1()));
    connect( serialPort2.serialPortObj, SIGNAL(readyRead()), this, SLOT(serialPortReadTransition2()));

    serialPort1.open("COM7");
    serialPort2.open("COM8");


    timer_pack1 = new QTimer(this);
    timer_pack2 = new QTimer(this);

    connect(timer_pack1,SIGNAL(timeout()),this,SLOT(test_com1()));
    timer_pack1->start(1000);
    connect(timer_pack2,SIGNAL(timeout()),this,SLOT(test_com2()));
    timer_pack2->start(500);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::serialPortReadTransition1()
{
        serialPort1.read();
}
void MainWindow::serialPortReadTransition2()
{
        serialPort2.read();
}


void MainWindow::test_com1()
{
    if( testOrder == 0 )
    {
        testOrder = 1;
        serialPort1.write((const char*)(test_pack.testArray),sizeof(test_pack.testArray));
    }
}

void MainWindow::test_com2()
{
    if( testOrder == 1 )
    {
        testOrder = 0;
        serialPort2.write((const char*)(test_pack2.testArray),sizeof(test_pack2.testArray));
    }

}
