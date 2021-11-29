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
        dataCollector(dataU8, serialPortIndex);
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

void serialPort::dataCollector(uint8_t data, uint8_t serialPortIndex)
{
    static uint8_t UartDataIndex[SERIALPORT_NB];
    static uint8_t UartDataPktLenght[SERIALPORT_NB];
    static uint8_t footerIndex[SERIALPORT_NB];
    static bool UartRxState[SERIALPORT_NB];

    if( data == 85 && UartDataIndex[serialPortIndex] == 0 && footerIndex[serialPortIndex] == 0 ) // 1. headerı bekler
    {
        UartRxDataBuffer[serialPortIndex][ UartDataIndex[serialPortIndex] ] = data;
        UartDataIndex[serialPortIndex]++;
    }
    else if( data == 85 && UartDataIndex[serialPortIndex] == 1 && footerIndex[serialPortIndex] == 0 ) // 2. headerı bekler
    {
        UartRxDataBuffer[serialPortIndex][ UartDataIndex[serialPortIndex] ] = data;
        UartDataIndex[serialPortIndex]++;
    }
    else if( UartDataIndex[serialPortIndex] == 2 && footerIndex[serialPortIndex] == 0 )
    {
        UartRxDataBuffer[serialPortIndex][ UartDataIndex[serialPortIndex] ] = data;
        UartDataIndex[serialPortIndex]++;
    }
    else if( UartDataIndex[serialPortIndex] == 3 && footerIndex[serialPortIndex] == 0 )
    {
        UartRxDataBuffer[serialPortIndex][ UartDataIndex[serialPortIndex] ] = data;
        UartDataIndex[serialPortIndex]++;
    }
    else if( UartDataIndex[serialPortIndex] == 4 && footerIndex[serialPortIndex] == 0 )
    {
        UartRxDataBuffer[serialPortIndex][ UartDataIndex[serialPortIndex] ] = data;
        UartDataPktLenght[serialPortIndex] = data - 7; // 7 olmasının sebebi 2 byte footer ve buraya gelene kadar zaten 5 tane datayı buffera yazdık. Yukarıda ki 4 tane ve bu state yani.
        UartDataIndex[serialPortIndex]++;
    }
    else if( UartDataPktLenght[serialPortIndex] > 0 && footerIndex[serialPortIndex] == 0 )
    {
        UartRxDataBuffer[serialPortIndex][ UartDataIndex[serialPortIndex] ] = data;
        UartDataPktLenght[serialPortIndex]--;
        UartDataIndex[serialPortIndex]++;
        if( UartDataPktLenght[serialPortIndex] == 0 )
        {
            UartRxState[serialPortIndex] = 1;
            footerIndex[serialPortIndex] = 1; // paket bitince footer bekliyor.
        }
    }
    else if( data == 170 && UartRxState[serialPortIndex] == 1 && footerIndex[serialPortIndex] == 1 ) // 1. footerı bekler
    {
        UartRxDataBuffer[serialPortIndex][ UartDataIndex[serialPortIndex] ] = data;
        UartDataIndex[serialPortIndex]++;
        footerIndex[serialPortIndex] = 2;
    }
    else if( data == 170 && UartRxState[serialPortIndex] == 1 && footerIndex[serialPortIndex] == 2 ) // 2. footerı bekler
    {
        UartRxDataBuffer[serialPortIndex][ UartDataIndex[serialPortIndex] ] = data;
        UartDataIndex[serialPortIndex] = 0;
        footerIndex[serialPortIndex] = 0;
        UartRxState[serialPortIndex] = 0;
        packetReady = 1;
    }
    else
    {
        footerIndex[serialPortIndex] = 0;
        UartDataIndex[serialPortIndex] = 0;
        UartDataPktLenght[serialPortIndex] = 0;
    }
}
