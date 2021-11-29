#include "serialport.h"

serialPort::serialPort()
{
    for(uint8_t i = 0; i < SERIALPORT_NB; i++ )
    {
        serialPortObj[i] = new QSerialPort;
        serialPortName[i] = "";
    }

    //Yaren burada kaç port varsa isimlerini yazıyoruz.
    //serialPortName[0] = "COM3";
    //serialPortName[1] = "COM2";
    //serialPortName[2] = "COM6";
}

void serialPort::open(uint8_t serialPortIndex)
{
    if( serialPortObj[serialPortIndex]->isOpen() )
    {
        qDebug("SERIAL PORT ALREADY OPENED") ;
        qDebug() << "error code = " << serialPortObj[serialPortIndex]->error();
        qDebug() << "error string = " << serialPortObj[serialPortIndex]->errorString();
    }
    else{
        qDebug()<<"SERIAL PORT OPENED...";
        serialPortObj[serialPortIndex]->setPortName(serialPortName[serialPortIndex]);
        serialPortObj[serialPortIndex]->open(QSerialPort::ReadWrite);
        serialPortObj[serialPortIndex]->setBaudRate(QSerialPort::Baud115200);
        serialPortObj[serialPortIndex]->setDataBits(QSerialPort::Data8);
        serialPortObj[serialPortIndex]->setParity(QSerialPort::NoParity);
        serialPortObj[serialPortIndex]->setStopBits(QSerialPort::OneStop);
        serialPortObj[serialPortIndex]->setFlowControl(QSerialPort::NoFlowControl);
    }
}

void serialPort::close(uint8_t serialPortIndex)
{
    if( serialPortObj[serialPortIndex]->isOpen() )
    {
       serialPortObj[serialPortIndex]->close();
    }
}

void serialPort::read(uint8_t serialPortIndex)
{
    QByteArray data = serialPortObj[serialPortIndex]->readAll();
    uint8_t dataU8;

    for(int i = 0; i < data.count(); i++)
    {
        dataU8 = static_cast<uint8_t>(data.at(i));
        //data_collector(dataU8);
        qDebug()<< dataU8;
    }

    //pkt_evaluate();
}

void serialPort::serialPort::write(char * arr, uint8_t size, uint8_t serialPortIndex)
{
    QByteArray data(arr, size);

    if( serialPortObj[serialPortIndex]->isWritable())
    {
        serialPortObj[serialPortIndex]->write(data);
    }
}

void serialPort::dataCollector(uint8_t data)
{

}
