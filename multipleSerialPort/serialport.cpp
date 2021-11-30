#include "serialport.h"
union test_pack_u test_pack;
union test_pack2_u test_pack2;
bool pck_status;
bool pck_status1;
bool pck_status2;
bool pck_status3;

serialPort::serialPort()
{
    test_pack.test.header = 0x5555;
    test_pack.test.pkt_direction=0x31;
    test_pack.test.pkt_ID = 0xE2;
    test_pack.test.pkt_size = 14;
    test_pack.test.test_value = 12.45;
    test_pack.test.CRC = 10000;
    test_pack.test.pkt_end = 0xAAAA;

    test_pack2.test2.header = 0x5555;
    test_pack2.test2.pkt_direction=0x31;
    test_pack2.test2.pkt_ID = 0xE1;
    test_pack2.test2.pkt_size = 14;
    test_pack2.test2.test_value = 12.45;
    test_pack2.test2.CRC = 10000;
    test_pack2.test2.pkt_end = 0xAAAA;

    serialPortObj = new QSerialPort;
}

void serialPort::open(QString serialPortName)
{
    if( serialPortObj->isOpen() )
    {
        qDebug("SERIAL PORT ALREADY OPENED") ;
        qDebug() << "error code = " << serialPortObj->error();
        qDebug() << "error string = " << serialPortObj->errorString();
    }
    else
    {
        qDebug()<<"SERIAL PORT OPENED...";
        serialPortObj->setPortName(serialPortName);
        serialPortObj->open(QSerialPort::ReadWrite);
        serialPortObj->setBaudRate(QSerialPort::Baud115200);
        serialPortObj->setDataBits(QSerialPort::Data8);
        serialPortObj->setParity(QSerialPort::NoParity);
        serialPortObj->setStopBits(QSerialPort::OneStop);
        serialPortObj->setFlowControl(QSerialPort::NoFlowControl);
    }
}

void serialPort::close(uint8_t serialPortIndex)
{
    if( serialPortObj->isOpen() )
    {
        serialPortObj->close();
    }
}

void serialPort::read()
{
    QByteArray data = serialPortObj->readAll();
    uint8_t dataU8;

    for(int i = 0; i < data.count(); i++)
    {
        dataU8 = static_cast<uint8_t>(data.at(i));
        dataCollector(dataU8);//
        qDebug()<< dataU8;
    }
    //pkt_evaluate(serialPortIndex);
}

void serialPort::write(const char * arr, uint8_t size)
{
    QByteArray data(arr, size);

    if( serialPortObj->isWritable())
    {
        serialPortObj->write(data);
    }
}

void serialPort::dataCollector(uint8_t data)
{
    static uint8_t UartDataIndex;
    static uint8_t UartDataPktLenght;
    static uint8_t footerIndex;
    static bool UartRxState;

    if( data == 85 && UartDataIndex == 0 && footerIndex == 0 ) // 1. headerı bekler
    {
        UartRxDataBuffer[ UartDataIndex ] = data;
        UartDataIndex++;
    }
    else if( data == 85 && UartDataIndex == 1 && footerIndex == 0 ) // 2. headerı bekler
    {
        UartRxDataBuffer[ UartDataIndex ] = data;
        UartDataIndex++;
    }
    else if( UartDataIndex == 2 && footerIndex == 0 )
    {
        UartRxDataBuffer[ UartDataIndex ] = data;
        UartDataIndex++;
    }
    else if( UartDataIndex == 3 && footerIndex == 0 )
    {
        UartRxDataBuffer[ UartDataIndex ] = data;
        UartDataIndex++;
    }
    else if( UartDataIndex == 4 && footerIndex == 0 )
    {
        UartRxDataBuffer[ UartDataIndex ] = data;
        UartDataPktLenght = data - 7; // 7 olmasının sebebi 2 byte footer ve buraya gelene kadar zaten 5 tane datayı buffera yazdık. Yukarıda ki 4 tane ve bu state yani.
        UartDataIndex++;
    }
    else if( UartDataPktLenght > 0 && footerIndex == 0 )
    {
        UartRxDataBuffer[ UartDataIndex ] = data;
        UartDataPktLenght--;
        UartDataIndex++;
        if( UartDataPktLenght == 0 )
        {
            UartRxState = 1;
            footerIndex = 1; // paket bitince footer bekliyor.
        }
    }
    else if( data == 170 && UartRxState == 1 && footerIndex == 1 ) // 1. footerı bekler
    {
        UartRxDataBuffer[ UartDataIndex ] = data;
        UartDataIndex++;
        footerIndex = 2;
    }
    else if( data == 170 && UartRxState == 1 && footerIndex == 2 ) // 2. footerı bekler
    {
        UartRxDataBuffer[ UartDataIndex ] = data;
        UartDataIndex = 0;
        footerIndex = 0;
        UartRxState = 0;
        packetReady = 1;
    }
    else
    {
        footerIndex = 0;
        UartDataIndex = 0;
        UartDataPktLenght = 0;
    }


}

void serialPort:: pkt_evaluate(uint8_t serialPortIndex)
{

}
